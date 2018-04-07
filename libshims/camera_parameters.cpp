#include <api1/client2/Parameters.h>

namespace android {
namespace camera2 {

bool Parameters::isFpsSupported(const __unused Vector<Size> &sizes, int __unused format, int32_t __unused fps) {
    return true;
}

}; // namespace camera2
}; // namespace android
