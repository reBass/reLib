// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include <algorithm>
#include <array>
#include <complex>
#include <gsl/span>

#include <re/lib/fft/real_fft.hpp>

namespace re {
namespace fft {

template <typename T, int N>
// Computes autocorrelation function of a given input
class acf
{
    static_assert(std::is_floating_point<T>::value);
public:
    void
    operator()(gsl::span<T, N> data)
    noexcept {
        operator()(data, data);
    }

    void
    operator()(gsl::span<T const, N> input, gsl::span<T, N> output)
    noexcept {
        std::array<T, 2*N> time_domain;
        std::copy(
            std::cbegin(input),
            std::cend(input),
            std::begin(time_domain)
        );
        std::fill(
            std::end(time_domain) - N,
            std::end(time_domain),
            0
        );

        std::array<std::complex<T>, N + 1> frequency_domain;
        fft(time_domain, frequency_domain);

        std::transform(
            std::cbegin(frequency_domain),
            std::cend(frequency_domain),
            std::begin(frequency_domain),
            [] (auto const& value) {
                return std::norm(value);
            }
        );

        ifft(frequency_domain, time_domain);

        auto lag = N;
        std::transform(
            std::cbegin(time_domain),
            std::cbegin(time_domain) + N,
            std::begin(output),
            [this, &lag] (auto value) {
                return std::abs(value) / (N * lag--);
            }
        );
    }

private:
    real_fft<T, 2*N, direction::forward> const fft;
    real_fft<T, 2*N, direction::inverse> const ifft;
};

} // fft
} // re
