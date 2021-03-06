//
//  main.cpp
//  xxHash-Test
//
//  Created by Daisuke T on 2019/02/15.
//  Copyright © 2019 xxHash-Test. All rights reserved.
//

#include <iostream>
#include <fstream>

#define XXH_STATIC_LINKING_ONLY   /* *_state_t */
#include "xxhash.h"

typedef uint8_t  BYTE;
typedef uint16_t U16;
typedef uint32_t U32;
typedef  int32_t S32;
typedef uint64_t U64;


// XXH32
static XXH32_hash_t xxh32(const char *input, unsigned int seed);
static void xxh32_oneshot();
static void xxh32_oneshot_with_seed();
static void xxh32_update();
static void xxh32_file(unsigned int seed);
static void xxh32_canonical();

// XXH64
static XXH64_hash_t xxh64(const char *input, unsigned int seed);
static void xxh64_oneshot();
static void xxh64_oneshot_with_seed();
static void xxh64_update();
static void xxh64_file(unsigned int seed);
static void xxh64_canonical();

// XXH3
static XXH64_hash_t xxh3_64(const char *input, unsigned int seed);
static void xxh3_64_oneshot();
static void xxh3_64_oneshot_with_seed();

static XXH128_hash_t xxh3_128(const char *input, unsigned int seed);
static void xxh3_128_oneshot();
static void xxh3_128_oneshot_with_seed();


int main(int argc, const char * argv[]) {

	xxh32_oneshot();
	xxh32_oneshot_with_seed();
	xxh32_update();
	xxh32_file(0);
	xxh32_file(0x7fffffff);
	xxh32_canonical();

	xxh64_oneshot();
	xxh64_oneshot_with_seed();
	xxh64_update();
	xxh64_file(0);
	xxh64_file(0x000000007fffffff);
	xxh64_canonical();

  xxh3_64_oneshot();
  xxh3_64_oneshot_with_seed();
  
  xxh3_128_oneshot();
  xxh3_128_oneshot_with_seed();
  
	return 0;
}



// MARK: - Vector
static const char * vector[] = {
	"",
	"1",
	"12",
	"123",
	"1234",
	"12345",
	"123456",
	"1234567",
	"12345678",
	"123456789",
	"123456789A",
	"123456789AB",
	"123456789ABC",
	"123456789ABCD",
	"123456789ABCDE",
	"123456789ABCDEF",
	"123456789ABCDEF1",
	"123456789ABCDEF12",
	"123456789ABCDEF123",
	"123456789ABCDEF1234",
	"123456789ABCDEF12345",
	"123456789ABCDEF123456",
	"123456789ABCDEF1234567",
	"123456789ABCDEF12345678",
	"123456789ABCDEF123456789",
	"123456789ABCDEF123456789A",
	"123456789ABCDEF123456789AB",
	"123456789ABCDEF123456789ABC",
	"123456789ABCDEF123456789ABCD",
	"123456789ABCDEF123456789ABCDE",
	"123456789ABCDEF123456789ABCDEF",
	"123456789ABCDEF123456789ABCDEF1",
	"123456789ABCDEF123456789ABCDEF12",
	"123456789ABCDEF123456789ABCDEF123",
	"123456789ABCDEF123456789ABCDEF1234",
	"123456789ABCDEF123456789ABCDEF12345",
	"123456789ABCDEF123456789ABCDEF123456",
	"123456789ABCDEF123456789ABCDEF1234567",
	"123456789ABCDEF123456789ABCDEF12345678",
	"123456789ABCDEF123456789ABCDEF123456789", // 39 chars
  "123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF", // 90 chars
  "123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF",  // 105 chars
  "123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF",  // 135 chars
 };

static const char * vector_2[] = {
	"",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
};



// MARK: - XXH method
static XXH32_hash_t xxh32(const char *input, unsigned int seed)
{
	return XXH32(input, strlen(input), seed);
}

static XXH64_hash_t xxh64(const char *input, unsigned int seed)
{
	return XXH64(input, strlen(input), seed);
}

static XXH64_hash_t xxh3_64(const char *input, unsigned int seed)
{
  return XXH3_64bits_withSeed(input, strlen(input), seed);
}

static XXH128_hash_t xxh3_128(const char *input, unsigned int seed)
{
  return XXH3_128bits_withSeed(input, strlen(input), seed);
}



// MARK: - One-shot
static void xxh32_oneshot()
{
	for(int i = 0; i < sizeof(vector) / sizeof(vector[0]); i++)
	{
		std::cout << "XCTAssertEqual(XXH32.digest(\"" << vector[i] << "\"), " << "0x" << std::hex << xxh32(vector[i], 0) << ")" << std::endl;
	}
  
  uint8_t hundredKB[1024 * 100];
  memset(hundredKB, UINT_MAX, 1024 * 100);
  std::cout << "XCTAssertEqual(XXH32.digest(hundredKB), " << "0x" << std::hex << XXH32(hundredKB, 1024 * 100, 0) << ")" << std::endl;
}

static void xxh32_oneshot_with_seed()
{
	for(int i = 0; i < sizeof(vector) / sizeof(vector[0]); i++)
	{
		std::cout << "XCTAssertEqual(XXH32.digest(\"" << vector[i] << "\", seed:" << 0 << "), " << "0x" << std::hex << xxh32(vector[i], 0) << ")" << std::endl;
		std::cout << "XCTAssertEqual(XXH32.digest(\"" << vector[i] << "\", seed:" << 1 << "), " << "0x" << std::hex << xxh32(vector[i], 1) << ")" << std::endl;
		std::cout << "XCTAssertEqual(XXH32.digest(\"" << vector[i] << "\", seed:" << "0x" << std::hex << 2147483647 << "), " << "0x" << std::hex << xxh32(vector[i], 2147483647) << ")" << std::endl;
	}
  
  uint8_t hundredKB[1024 * 100];
  memset(hundredKB, UINT_MAX, 1024 * 100);
  std::cout << "XCTAssertEqual(XXH32.digest(hundredKB, seed:" << 0 << "), " << "0x" << std::hex << XXH32(hundredKB, 1024 * 100, 0) << ")" << std::endl;
  std::cout << "XCTAssertEqual(XXH32.digest(hundredKB, seed:" << 1 << "), " << "0x" << std::hex << XXH32(hundredKB, 1024 * 100, 1) << ")" << std::endl;
  std::cout << "XCTAssertEqual(XXH32.digest(hundredKB, seed:" << "0x" << std::hex << 2147483647 << "), " << "0x" << std::hex << XXH32(hundredKB, 1024 * 100, 2147483647) << ")" << std::endl;
}

static void xxh64_oneshot()
{
	for(int i = 0; i < sizeof(vector) / sizeof(vector[0]); i++)
	{
		std::cout << "XCTAssertEqual(XXH64.digest(\"" << vector[i] << "\"), " << "0x" << std::hex << xxh64(vector[i], 0) << ")" << std::endl;
	}
  
  uint8_t hundredKB[1024 * 100];
  memset(hundredKB, UINT_MAX, 1024 * 100);
  std::cout << "XCTAssertEqual(XXH64.digest(hundredKB), " << "0x" << std::hex << XXH64(hundredKB, 1024 * 100, 0) << ")" << std::endl;
}

static void xxh64_oneshot_with_seed()
{
	for(int i = 0; i < sizeof(vector) / sizeof(vector[0]); i++)
	{
		std::cout << "XCTAssertEqual(XXH64.digest(\"" << vector[i] << "\", seed:" << 0 << "), " << "0x" << std::hex << xxh64(vector[i], 0) << ")" << std::endl;
		std::cout << "XCTAssertEqual(XXH64.digest(\"" << vector[i] << "\", seed:" << 1 << "), " << "0x" << std::hex << xxh64(vector[i], 1) << ")" << std::endl;
		std::cout << "XCTAssertEqual(XXH64.digest(\"" << vector[i] << "\", seed:" << "0x" << std::hex << 2147483647 << "), " << "0x" << std::hex << xxh64(vector[i], 2147483647) << ")" << std::endl;
	}
  
  uint8_t hundredKB[1024 * 100];
  memset(hundredKB, UINT_MAX, 1024 * 100);
  std::cout << "XCTAssertEqual(XXH64.digest(hundredKB, seed:" << 0 << "), " << "0x" << std::hex << XXH64(hundredKB, 1024 * 100, 0) << ")" << std::endl;
  std::cout << "XCTAssertEqual(XXH64.digest(hundredKB, seed:" << 1 << "), " << "0x" << std::hex << XXH64(hundredKB, 1024 * 100, 1) << ")" << std::endl;
  std::cout << "XCTAssertEqual(XXH64.digest(hundredKB, seed:" << "0x" << std::hex << 2147483647 << "), " << "0x" << std::hex << XXH64(hundredKB, 1024 * 100, 2147483647) << ")" << std::endl;
}

static void xxh3_64_oneshot()
{
  for(int i = 0; i < sizeof(vector) / sizeof(vector[0]); i++)
  {
    std::cout << "XCTAssertEqual(XXH3.digest64(\"" << vector[i] << "\"), " << "0x" << std::hex << xxh3_64(vector[i], 0) << ")" << std::endl;
  }

  uint8_t hundredKB[1024 * 100];
  memset(hundredKB, UINT_MAX, 1024 * 100);
  std::cout << "XCTAssertEqual(XXH3.digest64(hundredKB), " << "0x" << std::hex << XXH3_64bits_withSeed(hundredKB, 1024 * 100, 0) << ")" << std::endl;
}

static void xxh3_64_oneshot_with_seed()
{
  for(int i = 0; i < sizeof(vector) / sizeof(vector[0]); i++)
  {
    std::cout << "XCTAssertEqual(XXH3.digest64(\"" << vector[i] << "\", seed:" << 0 << "), " << "0x" << std::hex << xxh3_64(vector[i], 0) << ")" << std::endl;
    std::cout << "XCTAssertEqual(XXH3.digest64(\"" << vector[i] << "\", seed:" << 1 << "), " << "0x" << std::hex << xxh3_64(vector[i], 1) << ")" << std::endl;
    std::cout << "XCTAssertEqual(XXH3.digest64(\"" << vector[i] << "\", seed:" << "0x" << std::hex << 2147483647 << "), " << "0x" << std::hex << xxh3_64(vector[i], 2147483647) << ")" << std::endl;
  }

  uint8_t hundredKB[1024 * 100];
  memset(hundredKB, UINT_MAX, 1024 * 100);
  std::cout << "XCTAssertEqual(XXH3.digest64(hundredKB, seed:" << 0 << "), " << "0x" << std::hex << XXH3_64bits_withSeed(hundredKB, 1024 * 100, 0) << ")" << std::endl;
  std::cout << "XCTAssertEqual(XXH3.digest64(hundredKB, seed:" << 1 << "), " << "0x" << std::hex << XXH3_64bits_withSeed(hundredKB, 1024 * 100, 1) << ")" << std::endl;
  std::cout << "XCTAssertEqual(XXH3.digest64(hundredKB, seed:" << "0x" << std::hex << 2147483647 << "), " << "0x" << std::hex << XXH3_64bits_withSeed(hundredKB, 1024 * 100, 2147483647) << ")" << std::endl;
}

static void xxh3_128_oneshot()
{
  for(int i = 0; i < sizeof(vector) / sizeof(vector[0]); i++)
  {
    std::cout << "XCTAssertEqual(XXH3.digest128(\"" << vector[i] << "\")[0], " << "0x" << std::hex << xxh3_128(vector[i], 0).low64 << ")" << std::endl;
    std::cout << "XCTAssertEqual(XXH3.digest128(\"" << vector[i] << "\")[1], " << "0x" << std::hex << xxh3_128(vector[i], 0).high64 << ")" << std::endl;
  }
  
  uint8_t hundredKB[1024 * 100];
  memset(hundredKB, UINT_MAX, 1024 * 100);
  std::cout << "XCTAssertEqual(XXH3.digest128(hundredKB)[0], " << "0x" << std::hex << XXH3_128bits_withSeed(hundredKB, 1024 * 100, 0).low64 << ")" << std::endl;
  std::cout << "XCTAssertEqual(XXH3.digest128(hundredKB)[1], " << "0x" << std::hex << XXH3_128bits_withSeed(hundredKB, 1024 * 100, 0).high64 << ")" << std::endl;
}

static void xxh3_128_oneshot_with_seed()
{
  for(int i = 0; i < sizeof(vector) / sizeof(vector[0]); i++)
  {
    std::cout << "XCTAssertEqual(XXH3.digest128(\"" << vector[i] << "\", seed:" << 0 << ")[0], " << "0x" << std::hex << xxh3_128(vector[i], 0).low64 << ")" << std::endl;
    std::cout << "XCTAssertEqual(XXH3.digest128(\"" << vector[i] << "\", seed:" << 0 << ")[1], " << "0x" << std::hex << xxh3_128(vector[i], 0).high64 << ")" << std::endl;
    std::cout << "XCTAssertEqual(XXH3.digest128(\"" << vector[i] << "\", seed:" << 1 << ")[0], " << "0x" << std::hex << xxh3_128(vector[i], 1).low64 << ")" << std::endl;
    std::cout << "XCTAssertEqual(XXH3.digest128(\"" << vector[i] << "\", seed:" << 1 << ")[1], " << "0x" << std::hex << xxh3_128(vector[i], 1).high64 << ")" << std::endl;
    std::cout << "XCTAssertEqual(XXH3.digest128(\"" << vector[i] << "\", seed:" << "0x" << std::hex << 2147483647 << ")[0], " << "0x" << std::hex << xxh3_128(vector[i], 2147483647).low64 << ")" << std::endl;
    std::cout << "XCTAssertEqual(XXH3.digest128(\"" << vector[i] << "\", seed:" << "0x" << std::hex << 2147483647 << ")[1], " << "0x" << std::hex << xxh3_128(vector[i], 2147483647).high64 << ")" << std::endl;
  }
  
  uint8_t hundredKB[1024 * 100];
  memset(hundredKB, UINT_MAX, 1024 * 100);
  std::cout << "XCTAssertEqual(XXH3.digest128(hundredKB, seed:" << 0 << ")[0], " << "0x" << std::hex << XXH3_128bits_withSeed(hundredKB, 1024 * 100, 0).low64 << ")" << std::endl;
  std::cout << "XCTAssertEqual(XXH3.digest128(hundredKB, seed:" << 0 << ")[1], " << "0x" << std::hex << XXH3_128bits_withSeed(hundredKB, 1024 * 100, 0).high64 << ")" << std::endl;
  std::cout << "XCTAssertEqual(XXH3.digest128(hundredKB, seed:" << 1 << ")[0], " << "0x" << std::hex << XXH3_128bits_withSeed(hundredKB, 1024 * 100, 1).low64 << ")" << std::endl;
  std::cout << "XCTAssertEqual(XXH3.digest128(hundredKB, seed:" << 1 << ")[1], " << "0x" << std::hex << XXH3_128bits_withSeed(hundredKB, 1024 * 100, 1).high64 << ")" << std::endl;
  std::cout << "XCTAssertEqual(XXH3.digest128(hundredKB, seed:" << "0x" << std::hex << 2147483647 << ")[0], " << "0x" << std::hex << XXH3_128bits_withSeed(hundredKB, 1024 * 100, 2147483647).low64 << ")" << std::endl;
  std::cout << "XCTAssertEqual(XXH3.digest128(hundredKB, seed:" << "0x" << std::hex << 2147483647 << ")[1], " << "0x" << std::hex << XXH3_128bits_withSeed(hundredKB, 1024 * 100, 2147483647).high64 << ")" << std::endl;
}

// MARK: - Update
static void xxh32_update()
{
	XXH32_state_t *state = XXH32_createState();
	XXH32_reset(state, 0);
	
	for(int i = 0; i < sizeof(vector_2) / sizeof(vector_2[0]); i++)
	{
		XXH32_update(state, vector_2[i], strlen(vector_2[i]));
		std::cout << vector[i] << " " << "0x" << std::hex << XXH32_digest(state) << std::endl;
	}


	XXH32_reset(state, 0);
	XXH32_update(state, "123456789ABCDEF12", strlen("123456789ABCDEF12"));
	std::cout << "123456789ABCDEF12" << " " << "0x" << std::hex << XXH32_digest(state) << std::endl;

	XXH32_freeState(state);
}

static void xxh64_update()
{
	XXH64_state_t *state = XXH64_createState();
	XXH64_reset(state, 0);
	
	for(int i = 0; i < sizeof(vector_2) / sizeof(vector_2[0]); i++)
	{
		XXH64_update(state, vector_2[i], strlen(vector_2[i]));
		std::cout << vector[i] << " " << "0x" << std::hex << XXH64_digest(state) << std::endl;
	}
	
	
	XXH64_reset(state, 0);
	XXH64_update(state, "123456789ABCDEF12", strlen("123456789ABCDEF12"));
	std::cout << "123456789ABCDEF12" << " " << "0x" << std::hex << XXH64_digest(state) << std::endl;
	
	XXH64_freeState(state);
}



// MARK: - File
static void xxh32_file(unsigned int seed)
{
	std::fstream stream;

	stream.open("alice29.txt", std::ios::in);
	if(!stream.is_open())
	{
		std::cout <<  "file open error" << std::endl;
		return;
	}


	XXH32_state_t *state = XXH32_createState();
	XXH32_reset(state, seed);
	
	char buf[1024];
	while(!stream.eof()){
		stream.read(buf, sizeof(buf));

		XXH32_update(state, buf, stream.gcount());
	}
	std::cout << "alice29.txt" << " " << "0x" << std::hex << XXH32_digest(state) << std::endl;

	stream.close();
}

static void xxh64_file(unsigned int seed)
{
	std::fstream stream;
	
	stream.open("alice29.txt", std::ios::in);
	if(!stream.is_open())
	{
		std::cout <<  "file open error" << std::endl;
		return;
	}
	
	
	XXH64_state_t *state = XXH64_createState();
	XXH64_reset(state, seed);
	
	char buf[1024];
	while(!stream.eof()){
		stream.read(buf, sizeof(buf));
		
		XXH64_update(state, buf, stream.gcount());
	}
	std::cout << "alice29.txt" << " " << "0x" << std::hex << XXH64_digest(state) << std::endl;
	
	stream.close();
}



// MARK: - Canonical
static void xxh32_canonical()
{
	XXH32_hash_t hash = 0x01234567;
	XXH32_canonical_t canonical;

	XXH32_canonicalFromHash(&canonical, hash);
	std::cout << "0x" << std::hex << hash << " -> " << std::endl;
	std::cout << "    0x" << std::hex << +canonical.digest[0] << std::endl;
	std::cout << "    0x" << std::hex << +canonical.digest[1] << std::endl;
	std::cout << "    0x" << std::hex << +canonical.digest[2] << std::endl;
	std::cout << "    0x" << std::hex << +canonical.digest[3] << std::endl;

	hash = XXH32_hashFromCanonical(&canonical);
	std::cout << "-> 0x" << std::hex << hash << std::endl;
}

static void xxh64_canonical()
{
	XXH64_hash_t hash = 0x0123456789ABCDEF;
	XXH64_canonical_t canonical;
	
	XXH64_canonicalFromHash(&canonical, hash);
	std::cout << "0x" << std::hex << hash << " -> " << std::endl;
	std::cout << "    0x" << std::hex << +canonical.digest[0] << std::endl;
	std::cout << "    0x" << std::hex << +canonical.digest[1] << std::endl;
	std::cout << "    0x" << std::hex << +canonical.digest[2] << std::endl;
	std::cout << "    0x" << std::hex << +canonical.digest[3] << std::endl;
	std::cout << "    0x" << std::hex << +canonical.digest[4] << std::endl;
	std::cout << "    0x" << std::hex << +canonical.digest[5] << std::endl;
	std::cout << "    0x" << std::hex << +canonical.digest[6] << std::endl;
	std::cout << "    0x" << std::hex << +canonical.digest[7] << std::endl;

	hash = XXH64_hashFromCanonical(&canonical);
	std::cout << "-> 0x" << std::hex << hash << std::endl;
}

