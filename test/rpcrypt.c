// SPDX-License-Identifier: LicenseRef-AGPL-3.0-only-OpenSSL

#include <munit.h>

#include <chiaki/rpcrypt.h>

static MunitResult test_bright_ambassador_ps4_pre10(const MunitParameter params[], void *user)
{
	static const uint8_t nonce[] = { 0x43, 0x9, 0x67, 0xae, 0x36, 0x4b, 0x1c, 0x45, 0x26, 0x62, 0x37, 0x7a, 0xbf, 0x3f, 0xe9, 0x39 };
	static const uint8_t morning[] = { 0xd2, 0x78, 0x9f, 0x51, 0x85, 0xa7, 0x99, 0xa2, 0x44, 0x52, 0x77, 0x9c, 0x2b, 0x83, 0xcf, 0x7 };
	static const uint8_t bright_expected[] = { 0xa4, 0x4e, 0x2a, 0x16, 0x5e, 0x20, 0xd3, 0xf, 0xaa, 0x11, 0x8b, 0xc7, 0x7c, 0xa7, 0xdc, 0x11 };
	static const uint8_t ambassador_expected[] = { 0x1d, 0xa8, 0xb9, 0x1f, 0x6e, 0x26, 0x64, 0x2e, 0xbc, 0x8, 0x8b, 0x0, 0x4f, 0x1, 0x5b, 0x52 };

	uint8_t bright[CHIAKI_RPCRYPT_KEY_SIZE];
	uint8_t ambassador[CHIAKI_RPCRYPT_KEY_SIZE];
	chiaki_rpcrypt_bright_ambassador(CHIAKI_TARGET_PS4_9, bright, ambassador, nonce, morning);

	munit_assert_memory_equal(CHIAKI_RPCRYPT_KEY_SIZE, bright, bright_expected);
	munit_assert_memory_equal(CHIAKI_RPCRYPT_KEY_SIZE, ambassador, ambassador_expected);

	return MUNIT_OK;
}

static MunitResult test_bright_ambassador(const MunitParameter params[], void *user)
{
	static const uint8_t nonce_local[] = { 0xAE, 0x92, 0xE7, 0x64, 0x88, 0x26, 0x51, 0xEF, 0x89, 0x01, 0x8C, 0xFA, 0x69, 0x6C, 0x69, 0x38 };
	static const uint8_t morning_local[] = { 0x74, 0xA5, 0x9C, 0x96, 0x93, 0xC2, 0x08, 0x3B, 0xA6, 0xA8, 0x4B, 0xA0, 0x50, 0xFA, 0x8E, 0x5A };
	static const uint8_t ambassador_expected[] = { 0x92, 0xBE, 0xE2, 0x19, 0xB9, 0xD5, 0xB1, 0xAB, 0xC6, 0x49, 0x45, 0x77, 0xA4, 0x21, 0xE9, 0xBD };
	static const uint8_t bright_expected[] = { 0x67, 0x40, 0x8C, 0x5E, 0x65, 0x66, 0x5A, 0xD2, 0x91, 0xA8, 0x32, 0xEB, 0xE2, 0xD9, 0x0A, 0xBB };

	uint8_t bright[CHIAKI_RPCRYPT_KEY_SIZE];
	uint8_t ambassador[CHIAKI_RPCRYPT_KEY_SIZE];
	chiaki_rpcrypt_bright_ambassador(CHIAKI_TARGET_PS4_10, bright, ambassador, nonce_local, morning_local);

	munit_assert_memory_equal(CHIAKI_RPCRYPT_KEY_SIZE, ambassador, ambassador_expected);
	munit_assert_memory_equal(CHIAKI_RPCRYPT_KEY_SIZE, bright, bright_expected);

	return MUNIT_OK;
}

static MunitResult test_iv_ps4_pre10(const MunitParameter params[], void *user)
{
	static const uint8_t nonce[] = { 0x43, 0x9, 0x67, 0xae, 0x36, 0x4b, 0x1c, 0x45, 0x26, 0x62, 0x37, 0x7a, 0xbf, 0x3f, 0xe9, 0x39 };
	static const uint8_t morning[] = { 0xd2, 0x78, 0x9f, 0x51, 0x85, 0xa7, 0x99, 0xa2, 0x44, 0x52, 0x77, 0x9c, 0x2b, 0x83, 0xcf, 0x7 };
	static const uint8_t iv_a_expected[] = { 0x6, 0x29, 0xbe, 0x4, 0xe9, 0x91, 0x1c, 0x48, 0xb4, 0x5c, 0x2, 0x6d, 0xb7, 0xb7, 0x88, 0x46 };
	static const uint8_t iv_b_expected[] = { 0x3f, 0xd0, 0x83, 0xa, 0xc7, 0x30, 0xfc, 0x56, 0x75, 0x2d, 0xbe, 0xb8, 0x2c, 0x68, 0xa7, 0x4 };

	ChiakiRPCrypt rpcrypt;
	ChiakiErrorCode err;

	chiaki_rpcrypt_init_auth(&rpcrypt, CHIAKI_TARGET_PS4_9, nonce, morning);

	uint8_t iv[CHIAKI_RPCRYPT_KEY_SIZE];

	err = chiaki_rpcrypt_generate_iv(&rpcrypt, iv, 0);
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(CHIAKI_RPCRYPT_KEY_SIZE, iv, iv_a_expected);

	err = chiaki_rpcrypt_generate_iv(&rpcrypt, iv, 0);
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(CHIAKI_RPCRYPT_KEY_SIZE, iv, iv_a_expected);

	err = chiaki_rpcrypt_generate_iv(&rpcrypt, iv, 0x0102030405060708);
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(CHIAKI_RPCRYPT_KEY_SIZE, iv, iv_b_expected);

	err = chiaki_rpcrypt_generate_iv(&rpcrypt, iv, 0x0102030405060708);
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(CHIAKI_RPCRYPT_KEY_SIZE, iv, iv_b_expected);

	return MUNIT_OK;
}

static MunitResult test_iv_regist_ps4(const MunitParameter params[], void *user)
{
	static const uint8_t ambassador[] = { 0x3e, 0x7e, 0x7a, 0x82, 0x59, 0x73, 0xad, 0xab, 0x2f, 0x69, 0x43, 0x46, 0xbd, 0x44, 0xda, 0xb5 };
	static const uint8_t iv_expected[] = { 0xac, 0x48, 0x99, 0x77, 0xf9, 0x2a, 0xc5, 0x5b, 0xb9, 0x09, 0x3c, 0x33, 0xb6, 0x11, 0x3c, 0x46 };

	ChiakiRPCrypt rpcrypt;
	ChiakiErrorCode err;

	chiaki_rpcrypt_init_regist(&rpcrypt, CHIAKI_TARGET_PS4_10, ambassador, 0, 0);

	uint8_t iv[CHIAKI_RPCRYPT_KEY_SIZE];

	err = chiaki_rpcrypt_generate_iv(&rpcrypt, iv, 0);
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(CHIAKI_RPCRYPT_KEY_SIZE, iv, iv_expected);

	return MUNIT_OK;
}

static MunitResult test_iv_regist_ps5(const MunitParameter params[], void *user)
{
	static const uint8_t ambassador[] = { 0x3e, 0x7e, 0x7a, 0x82, 0x59, 0x73, 0xad, 0xab, 0x2f, 0x69, 0x43, 0x46, 0xbd, 0x44, 0xda, 0xb5 };
	static const uint8_t iv_expected[] = { 0x90, 0x44, 0x40, 0x82, 0x73, 0xf8, 0x04, 0x4d, 0xca, 0x76, 0x7b, 0x5a, 0x16, 0x39, 0x4d, 0x64 };

	ChiakiRPCrypt rpcrypt;
	ChiakiErrorCode err;

	chiaki_rpcrypt_init_regist(&rpcrypt, CHIAKI_TARGET_PS5_1, ambassador, 0, 0);

	uint8_t iv[CHIAKI_RPCRYPT_KEY_SIZE];

	err = chiaki_rpcrypt_generate_iv(&rpcrypt, iv, 0);
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(CHIAKI_RPCRYPT_KEY_SIZE, iv, iv_expected);

	return MUNIT_OK;
}

static MunitResult test_bright_regist_ps4(const MunitParameter params[], void *user)
{
	static const uint8_t ambassador[] = { 0xdc, 0xa1, 0xc8, 0x4d, 0xfe, 0x50, 0xd6, 0x57, 0x22, 0xda, 0x09, 0x65, 0x42, 0x31, 0xe7, 0xc2 };
	static const uint8_t bright_expected[] = { 0xed, 0xfc, 0x1d, 0xc5, 0xa2, 0xfe, 0x2d, 0x7f, 0x09, 0x19, 0x85, 0x75, 0x33, 0x6c, 0x13, 0x16 };
	size_t key_0_off = 0x1e;

	ChiakiRPCrypt rpcrypt;
	ChiakiErrorCode err;

	chiaki_rpcrypt_init_regist(&rpcrypt, CHIAKI_TARGET_PS4_10, ambassador, key_0_off, 78703893);
	munit_assert_memory_equal(CHIAKI_RPCRYPT_KEY_SIZE, rpcrypt.bright, bright_expected);

	return MUNIT_OK;
}

static MunitResult test_bright_regist_ps5(const MunitParameter params[], void *user)
{
	static const uint8_t ambassador[] = { 0xdc, 0xa1, 0xc8, 0x4d, 0xfe, 0x50, 0xd6, 0x57, 0x22, 0xda, 0x09, 0x65, 0x42, 0x31, 0xe7, 0xc2 };
	static const uint8_t bright_expected[] = { 0xe2, 0x9d, 0x64, 0x4c, 0x14, 0x1b, 0x9d, 0x61, 0x74, 0x31, 0xa5, 0x6d, 0x34, 0xcf, 0xc1, 0x7f };
	size_t key_0_off = 0x1e;

	ChiakiRPCrypt rpcrypt;
	ChiakiErrorCode err;

	chiaki_rpcrypt_init_regist(&rpcrypt, CHIAKI_TARGET_PS5_1, ambassador, key_0_off, 78703893);
	munit_assert_memory_equal(CHIAKI_RPCRYPT_KEY_SIZE, rpcrypt.bright, bright_expected);

	return MUNIT_OK;
}

static MunitResult test_encrypt_ps4_pre10(const MunitParameter params[], void *user)
{
	static const uint8_t nonce[] = { 0x43, 0x9, 0x67, 0xae, 0x36, 0x4b, 0x1c, 0x45, 0x26, 0x62, 0x37, 0x7a, 0xbf, 0x3f, 0xe9, 0x39 };
	static const uint8_t morning[] = { 0xd2, 0x78, 0x9f, 0x51, 0x85, 0xa7, 0x99, 0xa2, 0x44, 0x52, 0x77, 0x9c, 0x2b, 0x83, 0xcf, 0x7 };
	ChiakiRPCrypt rpcrypt;
	ChiakiErrorCode err;

	chiaki_rpcrypt_init_auth(&rpcrypt, CHIAKI_TARGET_PS4_9, nonce, morning);

	// less than block size
	uint8_t buf_a[] = { 0x13, 0x37, 0xc0, 0xff, 0xee };
	static const uint8_t cipher_expected_a[] = { 0x40, 0x48, 0x63, 0xeb, 0xb4 };
	err = chiaki_rpcrypt_encrypt(&rpcrypt, 0x0102030405060708, buf_a, buf_a, sizeof(buf_a));
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(sizeof(buf_a), buf_a, cipher_expected_a);

	// 1x block size
	uint8_t buf_b[] = { 0xdf, 0xa8, 0xa3, 0xd2, 0x6a, 0xd7, 0xf3, 0x4c, 0x4f, 0x87, 0x4c, 0xeb, 0x8c, 0x57, 0xfb, 0x3f };
	static const uint8_t cipher_expected_b[] = { 0x8c, 0xd7, 0x0, 0xc6, 0x30, 0x25, 0x3a, 0x18, 0x15, 0x20, 0xeb, 0x26, 0xf7, 0xed, 0xab, 0x15 };
	err = chiaki_rpcrypt_encrypt(&rpcrypt, 0x0102030405060708, buf_b, buf_b, sizeof(buf_b));
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(sizeof(buf_b), buf_b, cipher_expected_b);

	// more than block size, but not dividable by block size
	uint8_t buf_c[] = { 0x8, 0xd, 0x80, 0xc7, 0xb2, 0x2b, 0x9f, 0xf3, 0xe2, 0xd7, 0xc8, 0xa5, 0xb0, 0x92, 0xd5, 0x0, 0x8d, 0xe6, 0xd7, 0x74 };
	static const uint8_t cipher_expected_c[] = { 0x5b, 0x72, 0x23, 0xd3, 0xe8, 0xd9, 0x56, 0xa7, 0xb8, 0x70, 0x6f, 0x68, 0xcb, 0x28, 0x85, 0x2a, 0x6, 0xa5, 0xd2, 0xe0 };
	err = chiaki_rpcrypt_encrypt(&rpcrypt, 0x0102030405060708, buf_c, buf_c, sizeof(buf_c));
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(sizeof(buf_c), buf_c, cipher_expected_c);

	// 2x block size
	uint8_t buf_d[] = { 0xa1, 0x6, 0x91, 0x1b, 0x49, 0x74, 0xe0, 0x73, 0x3, 0xe8, 0x47, 0x42, 0x8, 0x4d, 0x83, 0x4e, 0xf3, 0x26, 0x14, 0x7b, 0xde, 0x2d, 0xf6, 0x7d, 0x47, 0x96, 0x8c, 0x3b, 0x66, 0x95, 0x7e, 0x5d };
	static const uint8_t cipher_expected_d[] = { 0xf2, 0x79, 0x32, 0xf, 0x13, 0x86, 0x29, 0x27, 0x59, 0x4f, 0xe0, 0x8f, 0x73, 0xf7, 0xd3, 0x64, 0x9d, 0x80, 0x7e, 0x90, 0xd1, 0xf5, 0xd8, 0x18, 0xe7, 0xbe, 0x16, 0x29, 0xfb, 0x48, 0x2b, 0xf8 };
	err = chiaki_rpcrypt_encrypt(&rpcrypt, 0x0102030405060708, buf_d, buf_d, sizeof(buf_d));
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(sizeof(buf_d), buf_d, cipher_expected_d);

	return MUNIT_OK;
}

static MunitResult test_decrypt_ps4_pre10(const MunitParameter params[], void *user)
{
	static const uint8_t nonce[] = { 0x43, 0x9, 0x67, 0xae, 0x36, 0x4b, 0x1c, 0x45, 0x26, 0x62, 0x37, 0x7a, 0xbf, 0x3f, 0xe9, 0x39 };
	static const uint8_t morning[] = { 0xd2, 0x78, 0x9f, 0x51, 0x85, 0xa7, 0x99, 0xa2, 0x44, 0x52, 0x77, 0x9c, 0x2b, 0x83, 0xcf, 0x7 };
	ChiakiRPCrypt rpcrypt;
	ChiakiErrorCode err;

	chiaki_rpcrypt_init_auth(&rpcrypt, CHIAKI_TARGET_PS4_9, nonce, morning);

	// less than block size
	uint8_t buf_a[] = { 0x8d, 0xd2, 0x1d, 0xfb };
	static const uint8_t expected_a[] = { 0xde, 0xad, 0xbe, 0xef };
	err = chiaki_rpcrypt_decrypt(&rpcrypt, 0x0102030405060708, buf_a, buf_a, sizeof(buf_a));
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(sizeof(buf_a), buf_a, expected_a);

	// 1x block size
	uint8_t buf_b[] = { 0xeb, 0x22, 0x4e, 0xb7, 0x73, 0x94, 0x6a, 0x31, 0x6e, 0xdd, 0xe5, 0x87, 0x29, 0xdc, 0xd5, 0x6b };
	static const uint8_t expected_b[] = { 0xb8, 0x5d, 0xed, 0xa3, 0x29, 0x66, 0xa3, 0x65, 0x34, 0x7a, 0x42, 0x4a, 0x52, 0x66, 0x85, 0x41 };
	err = chiaki_rpcrypt_decrypt(&rpcrypt, 0x0102030405060708, buf_b, buf_b, sizeof(buf_b));
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(sizeof(buf_b), buf_b, expected_b);

	// more than block size, but not dividable by block size
	uint8_t buf_c[] = { 0x2b, 0xd8, 0x1d, 0x86, 0x39, 0xe, 0x2d, 0xd7, 0xde, 0x2d, 0xb5, 0xbc, 0xba, 0x5e, 0xe9, 0x78, 0xee, 0xc9, 0x3b, 0x18 };
	static const uint8_t expected_c[] = { 0x78, 0xa7, 0xbe, 0x92, 0x63, 0xfc, 0xe4, 0x83, 0x84, 0x8a, 0x12, 0x71, 0xc1, 0xe4, 0xb9, 0x52, 0xf2, 0xbb, 0xcd, 0x39 };
	err = chiaki_rpcrypt_decrypt(&rpcrypt, 0x0102030405060708, buf_c, buf_c, sizeof(buf_c));
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(sizeof(buf_c), buf_c, expected_c);

	// 2x block size
	uint8_t buf_d[] = { 0x2b, 0x80, 0xda, 0xa0, 0x5e, 0x8d, 0x2e, 0x1d, 0xe1, 0x95, 0x16, 0xb1, 0xed, 0xd2, 0xc3, 0x8a, 0xb8, 0xcc, 0x6c, 0x42, 0x57, 0x8d, 0xc5, 0x7d, 0xf1, 0x5c, 0x4, 0x42, 0x97, 0x25, 0x3a, 0x91 };
	static const uint8_t expected_d[] = { 0x78, 0xff, 0x79, 0xb4, 0x4, 0x7f, 0xe7, 0x49, 0xbb, 0x32, 0xb1, 0x7c, 0x96, 0x68, 0x93, 0xa0, 0x86, 0xa6, 0x56, 0x38, 0x79, 0x2a, 0xa8, 0x3, 0x67, 0x9b, 0xf2, 0x82, 0x2d, 0x1a, 0x8, 0x29 };
	err = chiaki_rpcrypt_decrypt(&rpcrypt, 0x0102030405060708, buf_d, buf_d, sizeof(buf_d));
	if(err != CHIAKI_ERR_SUCCESS)
		return MUNIT_ERROR;
	munit_assert_memory_equal(sizeof(buf_d), buf_d, expected_d);

	return MUNIT_OK;
}


MunitTest tests_rpcrypt[] = {
	{
		"/bright_ambassador_ps4_pre10",
		test_bright_ambassador_ps4_pre10,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/bright_ambassador",
		test_bright_ambassador,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/iv_ps4_pre10",
		test_iv_ps4_pre10,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/iv_regist_ps4",
		test_iv_regist_ps4,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/iv_regist_ps5",
		test_iv_regist_ps5,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/bright_regist_ps4",
		test_bright_regist_ps4,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/bright_regist_ps5",
		test_bright_regist_ps5,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/encrypt_ps4_pre10",
		test_encrypt_ps4_pre10,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{
		"/decrypt_ps4_pre10",
		test_decrypt_ps4_pre10,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL
	},
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};
