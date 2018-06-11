// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include <cmath>

#include <algorithm>
#include <array>
#include <iterator>
#include <functional>
#include <gsl/span>

#include <re/lib/common.hpp>

namespace re {
namespace fft {

template <typename T, int_t N>
class hann_window
{
    static_assert(std::is_floating_point<T>::value);
public:
    hann_window()
    noexcept
    {
        encache();
    }

    template <class InputIt, class OutputIt>
    void
    cut(InputIt in, OutputIt out)
    const noexcept
    {
        std::transform(
            std::cbegin(cache),
            std::cend(cache),
            in,
            out,
            std::multiplies<>()
        );
    }

    void
    cut(gsl::span<T const, N> in, gsl::span<T, N> out)
    const noexcept
    {
        std::transform(
            std::cbegin(cache),
            std::cend(cache),
            std::cbegin(in),
            std::begin(out),
            std::multiplies<>()
        );
    }

    static constexpr T
    norm_correction()
    {
        return 0.5f;
    }

private:
    void
    encache()
    noexcept
    {
        for (auto i = 0; i < N; ++i) {
            cache[i] = window_function(i);
        }
    }

    static constexpr T
    window_function(int_t position)
    noexcept
    {
        auto relative_position = static_cast<T>(position) / N;
        return (1 - std::cos(relative_position * 2 * pi<T>)) / 2;
    }

    std::array<T, N> cache;
};

} // fft
} // re
