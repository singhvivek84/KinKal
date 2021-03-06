#ifndef KinKal_TPocaBase_hh
#define KinKal_TPocaBase_hh
//
// Untemplated base class for POCA calculations
//
#include "KinKal/Vectors.hh"
#include "KinKal/TTraj.hh"
#include <string>
#include <vector>
namespace KinKal {
  class TPocaBase {
    public:
      enum TPStat{converged=0,unconverged,pocafailed,derivfailed,invalid,unknown};
      static std::string const& statusName(TPStat status);
      //accessors
      Vec4 const& poca(size_t itraj) const { return poca_[itraj]; }
      Vec4 const& poca0() const { return poca_[0]; }
      Vec4 const& poca1() const { return poca_[1]; }
      double t0() const { return poca_[0].T(); }
      double t1() const { return poca_[1].T(); }
      TPStat status() const { return status_; }
      std::string const& statusName() const { return statusName(status_); }
      double doca() const { return doca_; } // DOCA signed by angular momentum
      double dDoca() const { return ddoca_; } // uncertainty on doca
      double precision() const { return precision_; }
      // utility functions
      void delta(Vec4& ds) const { ds = poca_[1]-poca_[0]; }
      double deltaT() const { return poca_[1].T() - poca_[0].T(); }
      void delta(Vec3& ds) const { ds = poca_[1].Vect()-poca_[0].Vect(); }
      bool usable() const { return status_ != pocafailed && status_ != unknown; }
      virtual TTraj const& ttraj0() const { return *ttraj_[0]; }
      virtual TTraj const& ttraj1() const { return *ttraj_[1]; }
      bool inRange() { return ttraj_[0]->inRange(poca_[0].T()) && ttraj_[1]->inRange(poca_[1].T()); }
      double dirDot() const { // dot product between directions at POCA
	Vec3 dir0, dir1;
	ttraj0().direction(t0(),dir0);
	ttraj1().direction(t1(),dir1);
	return dir0.Dot(dir1);
      }

      virtual ~TPocaBase(){}
    protected:
      TPStat status_; // status of computation
      double doca_; // geometric distance of closest approach, signed by angular momentunm
      double ddoca_; // geometric distance of closest approach, signed by angular momentunm
      double precision_; // precision used to define convergence
      std::array<Vec4,2> poca_; // spacetime points at GEOMETRIC closest approach
      std::array<const TTraj*,2> ttraj_; // base class pointers to trajs
      // override default constructor:
      // default precision = 10 um on DOCA
      TPocaBase(TTraj const& traj0,TTraj const& traj1, double precision=0.01) : status_(unknown), doca_(-1.0), ddoca_(-1.0), precision_(precision),
      ttraj_{&traj0,&traj1} {}
      // allow constructing an invalid object
      TPocaBase() : status_(invalid), doca_(-1.0), ddoca_(-1.0), precision_(0.0) {}
      void reset() {status_ = unknown;}
    private:
      static std::vector<std::string> statusNames_;
  };
}
#endif
