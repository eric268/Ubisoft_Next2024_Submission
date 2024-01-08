#include "pch.h"

//Math Tests
TEST(Vec4_Mul_M4x4, BMath)
{
	BMath::Vector4 vec(10.0f, -8.0f, 7.72f, -0.1f);

	BMath::Matrix4x4 m =
	{
		{   9.0f ,   0.3f ,    7.0f, -12.2f},
		{ -19.0f ,   0.87f, -107.1f,   0.0f},
		{  32.33f, -10.12f,  -72.5f, -12.2f},
		{  91.0f ,   1.13f,   -7.7f,  92.72f}
	};

	BMath::Vector4 result = m * vec;
	BMath::Vector4 ans(142.86, -1023.772, -154.22f, 832.244f);

	EXPECT_TRUE(BMath::Vector4::Equals(result, ans));
}

TEST(Vec4_Mul_M4x4_T2, BMath)
{
	BMath::Vector4 vec(1.0f, 1.0f, -1.0f, 1.0f);

	BMath::Matrix4x4 m =
	{
		{  1.0f ,   0.0f ,  0.0f,  0.0f},
		{  0.0f ,   1.0f,   0.0f,  0.0f},
		{  0.0f,	0.0f,   1.0f, -5.0f},
		{  0.0f ,   0.0f,   0.0f,  1.0f}
	};

	BMath::Vector4 result = m * vec;
	BMath::Vector4 ans(1.0f, 1.0f, -6.0f, 1.0f);

	EXPECT_TRUE(BMath::Vector4::Equals(result, ans));
}

TEST(M4x4_Mul_M4x4, BMath)
{
	BMath::Matrix4x4 m1 =
	{
		{  12.3f ,   10.13f,    5.15f,   2.2f} ,
		{   -9.0f, 1020.0f , -107.1f ,   0.0f} ,
		{  20.33f,   -0.12f,   -2.5f , 115.17f},
		{ -61.12f,    0.0f ,  -16.1f ,   0.72f}
	};

	BMath::Matrix4x4 m2 =
	{
		{   6.1f , 10.31f,     0.0f , -1003.145f},
		{  11.0f ,  1.87f,   -17.11f,     0.0f}  ,
		{ -12.11f,  9.12f,   -72.5f ,   -12.223f},
		{ 191.0f ,  1.0f ,    -0.7f ,     2.72f}
	};

	BMath::Matrix4x4 result = m1 * m2;

	BMath::Matrix4x4 ans =
	{
		{544.2935f, 194.9241f, -548.2393f, -12395.64795f},
		{12462.081f, 837.858f, -9687.45, 10337.3883 },
		{22150.438, 301.7479, 102.6842, -20050.11795},
		{-40.341, -776.2592, 1166.746, 61510.9711}
	};

	EXPECT_TRUE(BMath::Matrix4x4::Equals(result, ans));
}

TEST(CrossProd, BMath)
{
	BMath::Vector3 vec1(10.12f, -99.76, 0.13);
	BMath::Vector3 vec2(-1.45, 20.25, -102.12);

	BMath::Vector3 result = BMath::Vector3::Cross(vec1, vec2);
	BMath::Vector3 ans(10184.8587, 1033.2659, 60.278);

	EXPECT_TRUE(BMath::Vector3::Equals(result, ans));
}

TEST(Transpose_M4x4, BMath)
{
	BMath::Matrix4x4 m1 =
	{
		{1, 10, 11, 5},
		{-2, 3, 5, 17},
		{0, 9, 2, -2},
		{-4, -7, 11, 61}
	};

	BMath::Matrix4x4 result = BMath::Matrix4x4::Transpose(m1);

	BMath::Matrix4x4 ans =
	{
		{1, -2, 0, -4},
		{10, 3, 9, -7},
		{11, 5, 2, 11},
		{5, 17, -2, 61}
	};

	EXPECT_TRUE(BMath::Matrix4x4::Equals(result, ans));
}

TEST(Inverse_M4x4, BMath)
{
	BMath::Matrix4x4 m1 =
	{
		{1, 10, 11, 5},
		{-2, 3, 5, 17},
		{0, 9, 2, -2},
		{-4, -7, 11, 61}
	};

	BMath::Matrix4x4 result = BMath::Matrix4x4::Inverse(m1);

	BMath::Matrix4x4 ans =
	{
		{ 0.0888f  , -1.07f  , 0.4967f ,  0.3073f },
		{ -0.03089f, -0.075f , 0.1936f ,  0.0298f },
		{ 0.1197f  ,  0.2192f, -0.2681f, -0.0797f },
		{ -0.0193f , -0.1183f,  0.1031f,  0.0543f }
	};

	EXPECT_TRUE(BMath::Matrix4x4::Equals(result, ans));
}

TEST(Inverse_M4x4_2, BMath)
{
	BMath::Matrix4x4 m1 =
	{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, -10},
		{0, 0, 0, 1}
	};

	BMath::Matrix4x4 result = BMath::Matrix4x4::Inverse(m1);

	BMath::Matrix4x4 ans =
	{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 10},
		{0, 0, 0, 1}
	};

	EXPECT_TRUE(BMath::Matrix4x4::Equals(result, ans));
}

TEST(Quaternion_Axis_Angle, BMath)
{
	BMath::Quaternion q1(0.785f, BMath::Vector3(1.0f, 1.0f, 1.0f));
	BMath::Quaternion q2(0.392f, BMath::Vector3(0.0f, 0.707f, 0.707f));

	BMath::Quaternion a1(0.924f, 0.221f, 0.221f, 0.221f);
	BMath::Quaternion a2(0.981, 0.0f, 0.138f, 0.138f);

	EXPECT_TRUE(BMath::Quaternion::Equals(q1, a1) && BMath::Quaternion::Equals(q2, a2));
}

TEST(Quaternion_Mult, BMath)
{	
	BMath::Quaternion q(0.98f, 0.13f, 0.14f, 0.0f);
	BMath::Quaternion q2(0.92f, 0.0f, 0.0f,  0.38f);

	BMath::Quaternion result = q * q2;
	BMath::Quaternion ans(0.9016f, 0.1728f, 0.0794f, 0.3724f);


	EXPECT_TRUE(BMath::Quaternion::Equals(result, ans));
}

TEST(Quaternion_Mult_2, BMath)
{
	BMath::Quaternion q(0.88f, BMath::Vector3(1.0f, 1.5f, 0.3f));
	BMath::Quaternion q2(0.15f, BMath::Vector3(0.17f, 0.27f, 0.99f));

	BMath::Quaternion result = q * q2;
	BMath::Quaternion ans(0.8879f, 0.2667f, 0.3504f, 0.1343f);
	EXPECT_TRUE(BMath::Quaternion::Equals(result, ans));
}