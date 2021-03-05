// Yaşar Polatlı
// 250201075

#include <string>

namespace ceng391 {

typedef unsigned char uchar;

class Image {
public:
        Image(int width, int height, int n_channels, int step = 0);
        ~Image();

        static Image *new_gray(int width, int height);
        static Image *new_rgba(int width, int height);
        static Image *from_pnm(const std::string &filename);
        static Image *from_png(const std::string &filename, bool load_as_grayscale);
        static Image *from_deriv(int width, int height, const short *deriv);

        int w() const { return m_width; }
        int h() const { return m_height; }
        int n_ch() const { return m_n_channels; }

        uchar *data() { return m_data; }
        const uchar *data() const { return m_data; }
        uchar *data(int y) { return m_data + y * m_step; }
        const uchar *data(int y) const { return m_data + y * m_step; }

        uchar sample_bilinear(double x, double y, uchar default_value);

        void set_rect(int tlx, int tly, int width, int height, uchar value);
        void set_rect(int tlx, int tly, int width, int height,
                      uchar red, uchar green, uchar blue, uchar alpha);
        void set(uchar value) { set_rect(0, 0, m_width, m_height, value); }
        void set_zero() { set(0); }

        void to_grayscale();
        void to_rgba();

        // I = a * I + c
        void axpc(float multiplier, float offset);

        void box_filter_x(int filter_size);
        void box_filter_y(int filter_size);
        void box_filter  (int filter_size);

        void smooth_x(float sigma);
        void smooth_y(float sigma);
        void smooth  (float sigma);

        Image *filter_2d(int n, const float *K) const;

        short *deriv_x() const;
        short *deriv_y() const;

        void rotated(Image *Ir, double angle, bool rotate_by_center);
        void warp_affine(Image *Ia, const double *A,
                         const double *t, bool interpolate_bilinear);

        void save_as_pnm(const std::string &filename);
        bool load_pnm(const std::string &filename);

        bool save_as_png(const std::string &filename);
        bool load_png(const std::string &filename, bool load_as_grayscale);
private:
        void reallocate(int width, int height, int n_channels);
        uchar rgb_to_gray(uchar r, uchar g, uchar b) {
                int value = 0.3f * r + 0.59f * g + 0.11f * b;
                if (value < 0) {
                        value = 0;
                } else {
                        value = 255;
                }
                return value;
        }

        int m_width;
        int m_height;
        int m_n_channels;
        int m_step;
        uchar *m_data;
};

}
