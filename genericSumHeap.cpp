#include <iostream>
#include <cmath>
#include <vector>

// see general scan program in files
// we still need to do a scan, this is like the parallel sum, our reduce
// what is the init for a tally? for a  sum its 0 for  a  product its 1
// we do an init() for the scan
// compare()
// combine()
// we can make different types for tally and elem so that our interior and  leaf can have different types


template <typename ElemType, typename TallyType = ElemType>
class SumHeap {
  public:
    static const int ROOT = 0;
    typedef int ElemType;
    typedef std::vector<ElemType> Data;
    typedef std::vector<TallyType> TallyData;
    
    SumHeap (const Data *data) :  reduced(false), interior(nullptr), data(data),
                                  n(data->size()), height(ceil(log2(n))) {
      if ((n != 1 ) < height) {
        throw std::invalid_argument("Only Powers of 2 for now TODO");
      }
      intetior = new Data(n-1);
      reduce(ROOT);
    }


    ElemType getReduction(int i = ROOT) {
      reduced = reduced || reduce(ROOT);
      return value(i);
    }


    protected:
    virtual TallyType prepare(const ElemType& datum) {
      return datum;
    }
      virtual TallyType combine(const TallyType& left, const TallyType& right) {
        return left + right;
      }
      virtual bool reduce (int i) {
        if (!isLeaf(i)) {
          reduce (left (i));
          reduce (right (i));
          interior->at(i) = combine(left(i)), combine(right(i));
        }
      }

    private:
      Data*interior;
      const Data *data;
      int n;
      int height;

      ElemType value(int i) {
        if (i < n-1)
          return interior->at(i);
        else
          return data->at(i);
      }

      int size() {return n+(n-1);}
      int parent(int i) {return (i-1)/2;}
      int left (int i) {return (i*2 - 1);}
      int right (int i) {return (left(i)+1);}
      int isLeaf (int i) { return left(i)>= size ();}
};

class ProdReducer : SumHeap<int>
 {
   public:
    ProdReducer(SumHeap<int>::Data *data) : SumHeap<int>(data) {}

   protected:
    virtual int prepare (const int& datum) {
      return 1-datum;
    }
    virtual int combine (const int& left, const int& right) {
      return left * right;
    }
 };


 int main () {
   using namespace std;
   const int N = 1 << 4;
   vector<int> data(N);

   for (int i = 0; i < N; i++) {
    data[i] = rand() % 10 + 1;
    cout << data[i] << endl;
   }

  SumHeap reducer(&data);
  cout << reducer.getReduction() << endl;

  return 0;
 }