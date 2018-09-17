#ifndef ANIMATE_INTERPOLATE_HPP
#define ANIMATE_INTERPOLATE_HPP

// All templates in this file assert that t is in [0, 1].

template<typename T>
T linearInterpolate(float t, const T& val1, const T& val2, const float M[2][2]) {
    /*  T           M            G
     *         / m00  m01 \   / val1 \
     * [t 1] * |          | * |      |
     *         \ m10  m11 /   \ val2 /
     */
    float t0 = M[0][0] * t + M[1][0] * 1;
    float t1 = M[0][1] * t + M[1][1] * 1;

    return t0 * val1 + t1 * val2;
}

template<typename T>
T simpleLinearInterpolate(float t, const T& val1, const T& val2) {
    /*  T          M          G
     *         / -1  1 \   / val1 \
     * [t 1] * |       | * |      |
     *         \ 1   0 /   \ val2 /
     */
    const float linearM[2][2] = {
        {-1, 1},
        { 1, 0}
    };
    return linearInterpolate(t, val1, val2, linearM);
}

template<typename T>
T cubicInterpolate(float t, const T& val1, const T& val2, const T& val3, const T& val4, const float M[4][4]) {
    /*  T                       M                 G
     *                / m00  m01  m02  m03 \   / val1 \
     *                |                    |   |      |
     *   3  2         | m10  m11  m12  m13 |   | val2 |
     * [t  t  t  1] * |                    | * |      |
     *                | m20  m21  m22  m23 |   | val3 |
     *                |                    |   |      |
     *                \ m30  m31  m32  m33 /   \ val4 /
     */
    float t0 = M[0][0] * t * t * t + M[1][0] * t * t + M[2][0] * t + M[3][0] * 1;
    float t1 = M[0][1] * t * t * t + M[1][1] * t * t + M[2][1] * t + M[3][1] * 1;
    float t2 = M[0][2] * t * t * t + M[1][2] * t * t + M[2][2] * t + M[3][2] * 1;
    float t3 = M[0][3] * t * t * t + M[1][3] * t * t + M[2][3] * t + M[3][3] * 1;

    return t0 * val1 + t1 * val2 + t2 * val3 + t3 * val4;
}

template<typename T>
T catmullRomInterpolate(float t, const T& val1, const T& val2, const T& val3, const T& val4) {
    const float catmullRomM[4][4] = {
        { -0.5,  1.5, -1.5,  0.5 },
        {  1.0, -2.5,  2.0, -0.5 },
        { -0.5,  0.0,  0.5,  0.0 },
        {  0.0,  1.0,  0.0,  0.0 },
    };

    return cubicInterpolate(t, val1, val2, val3, val4, catmullRomM);
}

template<typename T, typename Iterator>
T catmullRomInterpolate(float t, Iterator begin) {
    return catmullRomInterpolate(t, *begin, *(begin + 1), *(begin + 2), *(begin + 3));
}

template<typename T>
T uniformNonrationalBasisSplineInterpolate(float t, const T& val1, const T& val2, const T& val3, const T& val4) {
    const float catmullRomM[4][4] = {
        { -1.0 / 6,  3.0 / 6, -3.0 / 6,  1.0 / 6 },
        {  3.0 / 6, -6.0 / 6,  3.0 / 6,  0.0 / 6 },
        { -3.0 / 6,  0.0 / 6,  3.0 / 6,  0.0 / 6 },
        {  1.0 / 6,  4.0 / 6,  1.0 / 6,  0.0 / 6 },
    };

    return cubicInterpolate(t, val1, val2, val3, val4, catmullRomM);
}

template<typename T, typename Iterator>
T uniformNonrationalBasisSplineInterpolate(float t, Iterator begin) {
    return uniformNonrationalBasisSplineInterpolate(t, *begin, *(begin + 1), *(begin + 2), *(begin + 3));
}

#endif // ANIMATE_INTERPOLATE_HPP