#ifndef ANIMATE_INTERPOLATE_HPP
#define ANIMATE_INTERPOLATE_HPP

static const float linearM[2][2] = {{-1,1}, {1,0}};

template<typename T>
T linearInterpolate(float t, const T& val1, const T& val2) {
    /*  T          M          G
     *         / -1  1 \   / val1 \
     * [t 1] * |       | * |      |
     *         \ 1   0 /   \ val2 /
     */
    float t0 = linearM[0][0] * t + linearM[1][0] * 1;
    float t1 = linearM[0][1] * t + linearM[1][1] * 1;

    return t0 * val1 + t1 * val2;
}

#endif // ANIMATE_INTERPOLATE_HPP