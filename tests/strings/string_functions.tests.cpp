#include <catch/catch.hpp>
#include <strings/string_functions.h>
#include <array_size.h>

using namespace strings;

TEST_CASE("char-char string copy functions", "[strings]")
{
    char buffer[32];
    const char *_31char = "1234567890123456789012345678901";
    const char *_32char = "12345678901234567890123456789012";

    SECTION("string should be copied")
    {
        CHECK(11 == string_copy(buffer, ArraySize(buffer), "some string"));
        REQUIRE(buffer == std::string("some string"));
    }

    SECTION("31 chars should be fully copied")
    {
        CHECK(31 == string_copy(buffer, ArraySize(buffer), _31char));
        REQUIRE(buffer == std::string(_31char));
    }

    SECTION("32 chars should copy 31 char + 0-termination")
    {
        CHECK(31 == string_copy(buffer, ArraySize(buffer), _32char));
        REQUIRE(buffer == std::string(_31char));
    }

    SECTION("copy empty string")
    {
        CHECK(0 == string_copy(buffer, ArraySize(buffer), ""));
        REQUIRE(buffer == std::string());
    }

    SECTION("copy when source len specified")
    {
        const char *_10char = "1234567890";
        char _30char[] = "123456789012345678901234567890";
        char _31char[] = "1234567890123456789012345678901";
        char _32char[] = "12345678901234567890123456789012";
        SECTION("10 chars should be copied")
        {
            CHECK(10 == string_copy(buffer, ArraySize(buffer), _30char, 10));
            REQUIRE(std::string(_10char) == buffer);
        }
        SECTION("30 chars should be copied")
        {
            CHECK(30 == string_copy(buffer, ArraySize(buffer), _30char, ArraySize(_30char)));
            REQUIRE(std::string(_30char) == buffer);
        }
        SECTION("31 chars should be copied")
        {
            CHECK(31 == string_copy(buffer, ArraySize(buffer), _31char, ArraySize(_31char)));
            REQUIRE(std::string(_31char) == buffer);
        }
        SECTION("32 chars should be cutted to 31 chars")
        {
            CHECK(31 == string_copy(buffer, ArraySize(buffer), _32char, ArraySize(_32char)));
            REQUIRE(std::string(_31char) == buffer);
        }
    }

    SECTION("invalid parameters tests")
    {
        SECTION("destination is null")
        {
            CHECK(0 == string_copy(static_cast<char*>(nullptr), ArraySize(buffer), ""));
        }

        SECTION("destination size is 0")
        {
            CHECK(0 == string_copy(buffer, 0, static_cast<char*>(nullptr)));
        }

        SECTION("source is null")
        {
            CHECK(0 == string_copy(buffer, ArraySize(buffer), static_cast<char*>(nullptr)));
            REQUIRE(buffer == std::string());
        }

        SECTION("source size is 0")
        {
            CHECK(0 == string_copy(buffer, ArraySize(buffer), "", 0));
            REQUIRE(buffer == std::string());
        }
    }
}

TEST_CASE("char-wchar_t string copy functions", "[strings]")
{
    char buffer[32];
    const char    _31char[]     =  "1234567890123456789012345678901";
    const wchar_t _31charWide[] = L"1234567890123456789012345678901";
    const char    _32char[]     =  "12345678901234567890123456789012";
    const wchar_t _32charWide[] = L"12345678901234567890123456789012";

    SECTION("string should be copied")
    {
        CHECK(11 == string_copy(buffer, ArraySize(buffer), L"some string"));
        REQUIRE(buffer == std::string("some string"));
    }

    SECTION("31 chars should be fully copied")
    {
        CHECK(31 == string_copy(buffer, ArraySize(buffer), _31charWide));
        REQUIRE(buffer == std::string(_31char));
    }

    SECTION("32 chars should copy 31 char + 0-termination")
    {
        CHECK(31 == string_copy(buffer, ArraySize(buffer), _32charWide));
        REQUIRE(buffer == std::string(_31char));
    }

    SECTION("copy empty string")
    {
        CHECK(0 == string_copy(buffer, ArraySize(buffer), ""));
        REQUIRE(buffer == std::string());
    }

    SECTION("copy when source len specified")
    {
        const char *_10char = "1234567890";
        char    _30char[]     =  "123456789012345678901234567890";
        wchar_t _30charWide[] = L"123456789012345678901234567890";
        SECTION("10 chars should be copied")
        {
            CHECK(10 == string_copy(buffer, ArraySize(buffer), _30charWide, 10));
            REQUIRE(std::string(_10char) == buffer);
        }
        SECTION("30 chars should be copied")
        {
            CHECK(30 == string_copy(buffer, ArraySize(buffer), _30charWide, ArraySize(_30charWide)));
            REQUIRE(std::string(_30char) == buffer);
        }
        SECTION("31 chars should be copied")
        {
            CHECK(31 == string_copy(buffer, ArraySize(buffer), _31charWide, ArraySize(_31charWide)));
            REQUIRE(std::string(_31char) == buffer);
        }
        SECTION("32 chars should be cutted to 31 chars")
        {
            CHECK(31 == string_copy(buffer, ArraySize(buffer), _32charWide, ArraySize(_32charWide)));
            REQUIRE(std::string(_31char) == buffer);
        }
    }

    SECTION("invalid parameters tests")
    {
        SECTION("destination is null")
        {
            CHECK(0 == string_copy(static_cast<char*>(nullptr), ArraySize(buffer), L""));
        }

        SECTION("destination size is 0")
        {
            CHECK(0 == string_copy(buffer, 0, static_cast<wchar_t*>(nullptr)));
        }

        SECTION("source is null")
        {
            CHECK(0 == string_copy(buffer, ArraySize(buffer), static_cast<wchar_t*>(nullptr)));
            REQUIRE(buffer == std::string());
        }

        SECTION("source size is 0")
        {
            CHECK(0 == string_copy(buffer, ArraySize(buffer), L"", 0));
            REQUIRE(buffer == std::string());
        }
    }
}

TEST_CASE("wchar_t-char string copy functions", "[strings]")
{
    wchar_t buffer[32];
    const char    _31char[]     =  "1234567890123456789012345678901";
    const wchar_t _31charWide[] = L"1234567890123456789012345678901";
    const char    _32char[]     =  "12345678901234567890123456789012";
    const wchar_t _32charWide[] = L"12345678901234567890123456789012";

    SECTION("string should be copied")
    {
        CHECK(11 == string_copy(buffer, ArraySize(buffer), "some string"));
        REQUIRE(buffer == std::wstring(L"some string"));
    }

    SECTION("31 chars should be fully copied")
    {
        CHECK(31 == string_copy(buffer, ArraySize(buffer), _31char));
        REQUIRE(buffer == std::wstring(_31charWide));
    }

    SECTION("32 chars should copy 31 char + 0-termination")
    {
        CHECK(31 == string_copy(buffer, ArraySize(buffer), _32char));
        REQUIRE(buffer == std::wstring(_31charWide));
    }

    SECTION("copy empty string")
    {
        CHECK(0 == string_copy(buffer, ArraySize(buffer), ""));
        REQUIRE(buffer == std::wstring());
    }

    SECTION("copy when source len specified")
    {
        const wchar_t *_10char = L"1234567890";
        char     _30char[]     =  "123456789012345678901234567890";
        wchar_t  _30charWide[] = L"123456789012345678901234567890";
        SECTION("10 chars should be copied")
        {
            CHECK(10 == string_copy(buffer, ArraySize(buffer), _30char, 10));
            REQUIRE(std::wstring(_10char) == buffer);
        }
        SECTION("30 chars should be copied")
        {
            CHECK(30 == string_copy(buffer, ArraySize(buffer), _30char, ArraySize(_30char)));
            REQUIRE(std::wstring(_30charWide) == buffer);
        }
        SECTION("31 chars should be copied")
        {
            CHECK(31 == string_copy(buffer, ArraySize(buffer), _31char, ArraySize(_31char)));
            REQUIRE(std::wstring(_31charWide) == buffer);
        }
        SECTION("32 chars should be cutted to 31 chars")
        {
            CHECK(31 == string_copy(buffer, ArraySize(buffer), _32char, ArraySize(_32char)));
            REQUIRE(std::wstring(_31charWide) == buffer);
        }
    }

    SECTION("invalid parameters tests")
    {
        SECTION("destination is null")
        {
            CHECK(0 == string_copy(static_cast<char*>(nullptr), ArraySize(buffer), ""));
        }

        SECTION("destination size is 0")
        {
            CHECK(0 == string_copy(buffer, 0, static_cast<char*>(nullptr)));
        }

        SECTION("source is null")
        {
            CHECK(0 == string_copy(buffer, ArraySize(buffer), static_cast<char*>(nullptr)));
            REQUIRE(buffer == std::wstring());
        }

        SECTION("source size is 0")
        {
            CHECK(0 == string_copy(buffer, ArraySize(buffer), "", 0));
            REQUIRE(buffer == std::wstring());
        }
    }
}

TEST_CASE("wchar_t-wchar_t string copy functions", "[strings]")
{
    wchar_t buffer[32];
    const wchar_t _31char[] = L"1234567890123456789012345678901";
    const wchar_t _32char[] = L"12345678901234567890123456789012";

    SECTION("string should be copied")
    {
        CHECK(11 == string_copy(buffer, ArraySize(buffer), L"some string"));
        REQUIRE(buffer == std::wstring(L"some string"));
    }

    SECTION("31 chars should be fully copied")
    {
        CHECK(31 == string_copy(buffer, ArraySize(buffer), _31char));
        REQUIRE(buffer == std::wstring(_31char));
    }

    SECTION("32 chars should copy 31 char + 0-termination")
    {
        CHECK(31 == string_copy(buffer, ArraySize(buffer), _32char));
        REQUIRE(buffer == std::wstring(_31char));
    }

    SECTION("copy empty string")
    {
        CHECK(0 == string_copy(buffer, ArraySize(buffer), L""));
        REQUIRE(buffer == std::wstring());
    }

    SECTION("copy when source len specified")
    {
        const wchar_t *_10char = L"1234567890";
        wchar_t _30char[] = L"123456789012345678901234567890";
        SECTION("10 chars should be copied")
        {
            CHECK(10 == string_copy(buffer, ArraySize(buffer), _30char, 10));
            REQUIRE(std::wstring(_10char) == buffer);
        }
        SECTION("30 chars should be copied")
        {
            CHECK(30 == string_copy(buffer, ArraySize(buffer), _30char, ArraySize(_30char)));
            REQUIRE(std::wstring(_30char) == buffer);
        }
        SECTION("31 chars should be copied")
        {
            CHECK(31 == string_copy(buffer, ArraySize(buffer), _31char, ArraySize(_31char)));
            REQUIRE(std::wstring(_31char) == buffer);
        }
        SECTION("32 chars should be cutted to 31 chars")
        {
            CHECK(31 == string_copy(buffer, ArraySize(buffer), _32char, ArraySize(_32char)));
            REQUIRE(std::wstring(_31char) == buffer);
        }
    }

    SECTION("invalid parameters tests") {
        SECTION("destination is null") {
            CHECK(0 == string_copy(static_cast<char *>(nullptr), ArraySize(buffer), L""));
        }

        SECTION("destination size is 0") {
            CHECK(0 == string_copy(buffer, 0, static_cast<wchar_t *>(nullptr)));
        }

        SECTION("source is null") {
            CHECK(0 == string_copy(buffer, ArraySize(buffer), static_cast<wchar_t *>(nullptr)));
            REQUIRE(buffer == std::wstring());
        }

        SECTION("source size is 0") {
            CHECK(0 == string_copy(buffer, ArraySize(buffer), L"", 0));
            REQUIRE(buffer == std::wstring());
        }
    }
}

TEST_CASE("buffer to string tests")
{
    uint8_t bytes[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    SECTION("char buffer")
    {
        char buffer[1024] {};
        SECTION("default delimiter") {
            std::string expectedFullBytes{"00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10"};
            std::string expectedSmallBuffer{"00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E..."};

            SECTION("should convert bytes to string when buffer is enough") {
                size_t result = buffer_to_string(buffer, ArraySize(buffer), bytes, ArraySize(bytes));
                CHECK(result == expectedFullBytes.size());
                REQUIRE(expectedFullBytes == buffer);
            }

            SECTION("should convert bytes to string when buffer is for exact fit") {
                size_t result = buffer_to_string(buffer, expectedFullBytes.size() + 1, bytes, ArraySize(bytes));
                CHECK(result == expectedFullBytes.size());
                REQUIRE(expectedFullBytes == buffer);
            }

            SECTION("should emplace ... when buffer is less than exact fit") {
                size_t result = buffer_to_string(buffer, expectedFullBytes.size(), bytes, ArraySize(bytes));
                CHECK(result == expectedSmallBuffer.size());
                REQUIRE(expectedSmallBuffer == buffer);
            }
        }

        SECTION("dash delimiter")
        {
            std::string expectedFullBytes { "00-01-02-03-04-05-06-07-08-09-0A-0B-0C-0D-0E-0F-10" };
            std::string expectedSmallBuffer { "00-01-02-03-04-05-06-07-08-09-0A-0B-0C-0D-0E..." };
            char delimiter = '-';

            SECTION("should convert bytes to string when buffer is enough") {
                size_t result = buffer_to_string(buffer, ArraySize(buffer), bytes, ArraySize(bytes), delimiter);
                CHECK(result == expectedFullBytes.size());
                REQUIRE(expectedFullBytes == buffer);
            }

            SECTION("should convert bytes to string when buffer is for exact fit") {
                size_t result = buffer_to_string(buffer, expectedFullBytes.size() + 1, bytes, ArraySize(bytes), delimiter);
                CHECK(result == expectedFullBytes.size());
                REQUIRE(expectedFullBytes == buffer);
            }

            SECTION("should emplace ... when buffer is less than exact fit") {
                size_t result = buffer_to_string(buffer, expectedFullBytes.size(), bytes, ArraySize(bytes), delimiter);
                CHECK(result == expectedSmallBuffer.size());
                REQUIRE(expectedSmallBuffer == buffer);
            }
        }

        SECTION("empty delimiter")
        {
            std::string expectedFullBytes   { "000102030405060708090A0B0C0D0E0F10" };
            std::string expectedSmallBuffer { "000102030405060708090A0B0C0D0E..." };
            char delimiter = 0;

            SECTION("should convert bytes to string when buffer is enough") {
                size_t result = buffer_to_string(buffer, ArraySize(buffer), bytes, ArraySize(bytes), delimiter);
                CHECK(result == expectedFullBytes.size());
                REQUIRE(expectedFullBytes == buffer);
            }

            SECTION("should convert bytes to string when buffer is for exact fit") {
                size_t result = buffer_to_string(buffer, expectedFullBytes.size() + 1, bytes, ArraySize(bytes), delimiter);
                CHECK(result == expectedFullBytes.size());
                REQUIRE(expectedFullBytes == buffer);
            }

            SECTION("should emplace ... when buffer is less than exact fit") {
                size_t result = buffer_to_string(buffer, expectedFullBytes.size(), bytes, ArraySize(bytes), delimiter);
                CHECK(result == expectedSmallBuffer.size());
                REQUIRE(expectedSmallBuffer == buffer);
            }

        }

        SECTION("invalid parameters")
        {
            std::string emptyString = {};
            std::string manyFiller = "...";

            SECTION("nullptr buffer should return 0") {
                REQUIRE(0 == buffer_to_string(static_cast<char *>(nullptr), 16, bytes, ArraySize(bytes)));
            }

            SECTION("nullptr array should produce empty string") {
                CHECK(0 == buffer_to_string(buffer, ArraySize(buffer), static_cast<uint8_t *>(nullptr), 16));
                REQUIRE(emptyString == buffer);
            }

            SECTION("without delimiter") {
                char delimiter = 0;
                SECTION("empty buffer should produce empty string");
                {
                    size_t result = buffer_to_string(buffer, 0, bytes, ArraySize(bytes), delimiter);
                    CHECK(result == 0);
                    REQUIRE(emptyString == buffer);
                }

                SECTION("small buffer should produce empty string");
                {
                    size_t result = buffer_to_string(buffer, 3, bytes, ArraySize(bytes), delimiter);
                    CHECK(result == 0);
                    REQUIRE(emptyString == buffer);
                }

                SECTION("small buffer should produce ... string");
                {
                    size_t result = buffer_to_string(buffer, 4, bytes, ArraySize(bytes), delimiter);
                    CHECK(result == 3);
                    REQUIRE(manyFiller == buffer);
                }

                SECTION("empty array should produce empty string") {
                    size_t result = buffer_to_string(buffer, ArraySize(buffer), bytes, 0, delimiter);
                    CHECK(result == 0);
                    REQUIRE(emptyString == buffer);
                }
            }

            SECTION("dash delimiter") {
                char delimiter = '-';
                SECTION("empty buffer should produce empty string");
                {
                    size_t result = buffer_to_string(buffer, 0, bytes, ArraySize(bytes), delimiter);
                    CHECK(result == 0);
                    REQUIRE(emptyString == buffer);
                }

                SECTION("small buffer should produce empty string");
                {
                    size_t result = buffer_to_string(buffer, 3, bytes, ArraySize(bytes), delimiter);
                    CHECK(result == 0);
                    REQUIRE(emptyString == buffer);
                }

                SECTION("small buffer should produce ... string");
                {
                    size_t result = buffer_to_string(buffer, 4, bytes, ArraySize(bytes), delimiter);
                    CHECK(result == 3);
                    REQUIRE(manyFiller == buffer);
                }

                SECTION("empty array should produce empty string") {
                    size_t result = buffer_to_string(buffer, ArraySize(buffer), bytes, 0, delimiter);
                    CHECK(result == 0);
                    REQUIRE(emptyString == buffer);
                }
            }

        }
    }

    SECTION("wchar_t buffer")
    {
        wchar_t buffer[1024] {};
        SECTION("default delimiter") {
            std::wstring expectedFullBytes{L"00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10"};
            std::wstring expectedSmallBuffer{L"00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E..."};

            SECTION("should convert bytes to string when buffer is enough") {
                size_t result = buffer_to_string(buffer, ArraySize(buffer), bytes, ArraySize(bytes));
                CHECK(result == expectedFullBytes.size());
                REQUIRE(expectedFullBytes == buffer);
            }

            SECTION("should convert bytes to string when buffer is for exact fit") {
                size_t result = buffer_to_string(buffer, expectedFullBytes.size() + 1, bytes, ArraySize(bytes));
                CHECK(result == expectedFullBytes.size());
                REQUIRE(expectedFullBytes == buffer);
            }

            SECTION("should emplace ... when buffer is less than exact fit") {
                size_t result = buffer_to_string(buffer, expectedFullBytes.size(), bytes, ArraySize(bytes));
                CHECK(result == expectedSmallBuffer.size());
                REQUIRE(expectedSmallBuffer == buffer);
            }
        }

        SECTION("dash delimiter")
        {
            std::wstring expectedFullBytes { L"00-01-02-03-04-05-06-07-08-09-0A-0B-0C-0D-0E-0F-10" };
            std::wstring expectedSmallBuffer { L"00-01-02-03-04-05-06-07-08-09-0A-0B-0C-0D-0E..." };
            wchar_t delimiter = L'-';

            SECTION("should convert bytes to string when buffer is enough") {
                size_t result = buffer_to_string(buffer, ArraySize(buffer), bytes, ArraySize(bytes), delimiter);
                CHECK(result == expectedFullBytes.size());
                REQUIRE(expectedFullBytes == buffer);
            }

            SECTION("should convert bytes to string when buffer is for exact fit") {
                size_t result = buffer_to_string(buffer, expectedFullBytes.size() + 1, bytes, ArraySize(bytes), delimiter);
                CHECK(result == expectedFullBytes.size());
                REQUIRE(expectedFullBytes == buffer);
            }

            SECTION("should emplace ... when buffer is less than exact fit") {
                size_t result = buffer_to_string(buffer, expectedFullBytes.size(), bytes, ArraySize(bytes), delimiter);
                CHECK(result == expectedSmallBuffer.size());
                REQUIRE(expectedSmallBuffer == buffer);
            }
        }

        SECTION("empty delimiter")
        {
            std::wstring expectedFullBytes   { L"000102030405060708090A0B0C0D0E0F10" };
            std::wstring expectedSmallBuffer { L"000102030405060708090A0B0C0D0E..." };
            wchar_t delimiter = 0;

            SECTION("should convert bytes to string when buffer is enough") {
                size_t result = buffer_to_string(buffer, ArraySize(buffer), bytes, ArraySize(bytes), delimiter);
                CHECK(result == expectedFullBytes.size());
                REQUIRE(expectedFullBytes == buffer);
            }

            SECTION("should convert bytes to string when buffer is for exact fit") {
                size_t result = buffer_to_string(buffer, expectedFullBytes.size() + 1, bytes, ArraySize(bytes), delimiter);
                CHECK(result == expectedFullBytes.size());
                REQUIRE(expectedFullBytes == buffer);
            }

            SECTION("should emplace ... when buffer is less than exact fit") {
                size_t result = buffer_to_string(buffer, expectedFullBytes.size(), bytes, ArraySize(bytes), delimiter);
                CHECK(result == expectedSmallBuffer.size());
                REQUIRE(expectedSmallBuffer == buffer);
            }

        }

        SECTION("invalid parameters")
        {
            std::wstring emptyString = {};
            std::wstring manyFiller = L"...";

            SECTION("nullptr buffer should return 0") {
                REQUIRE(0 == buffer_to_string(static_cast<char *>(nullptr), 16, bytes, ArraySize(bytes)));
            }

            SECTION("nullptr array should produce empty string") {
                CHECK(0 == buffer_to_string(buffer, ArraySize(buffer), static_cast<uint8_t *>(nullptr), 16));
                REQUIRE(emptyString == buffer);
            }

            SECTION("without delimiter") {
                wchar_t delimiter = 0;
                SECTION("empty buffer should produce empty string");
                {
                    size_t result = buffer_to_string(buffer, 0, bytes, ArraySize(bytes), delimiter);
                    CHECK(result == 0);
                    REQUIRE(emptyString == buffer);
                }

                SECTION("small buffer should produce empty string");
                {
                    size_t result = buffer_to_string(buffer, 3, bytes, ArraySize(bytes), delimiter);
                    CHECK(result == 0);
                    REQUIRE(emptyString == buffer);
                }

                SECTION("small buffer should produce ... string");
                {
                    size_t result = buffer_to_string(buffer, 4, bytes, ArraySize(bytes), delimiter);
                    CHECK(result == 3);
                    REQUIRE(manyFiller == buffer);
                }

                SECTION("empty array should produce empty string") {
                    size_t result = buffer_to_string(buffer, ArraySize(buffer), bytes, 0, delimiter);
                    CHECK(result == 0);
                    REQUIRE(emptyString == buffer);
                }
            }

            SECTION("dash delimiter") {
                wchar_t delimiter = L'-';
                SECTION("empty buffer should produce empty string");
                {
                    size_t result = buffer_to_string(buffer, 0, bytes, ArraySize(bytes), delimiter);
                    CHECK(result == 0);
                    REQUIRE(emptyString == buffer);
                }

                SECTION("small buffer should produce empty string");
                {
                    size_t result = buffer_to_string(buffer, 3, bytes, ArraySize(bytes), delimiter);
                    CHECK(result == 0);
                    REQUIRE(emptyString == buffer);
                }

                SECTION("small buffer should produce ... string");
                {
                    size_t result = buffer_to_string(buffer, 4, bytes, ArraySize(bytes), delimiter);
                    CHECK(result == 3);
                    REQUIRE(manyFiller == buffer);
                }

                SECTION("empty array should produce empty string") {
                    size_t result = buffer_to_string(buffer, ArraySize(buffer), bytes, 0, delimiter);
                    CHECK(result == 0);
                    REQUIRE(emptyString == buffer);
                }
            }
        }
    }
}
