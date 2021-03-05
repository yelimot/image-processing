// M. Yaşar Polatlı
// 250201075

// Probably there are bugs in the code.
// I couldn't test and fix it because of the density of my exam and homework schedule, my time didn't enough to accomplish.

#include "image.hpp"

namespace ceng391 {

class ImagePyr {

public:
		ImagePyr(int m_n_levels, Image *img, float sigma);
		~Image();

		// getter
		int lvl() const { return m_n_levels; }

		// getter for ith level
		Image *ithlvl(int lvl_no) { return &m_levels[lvl_no]; }

		void cons_pyr();

private:
		float sigma;
		int m_n_levels;
		Image *m_levels;

}

}