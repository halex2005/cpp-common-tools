﻿#include <catch/catch.hpp>
#include <strings/formatter.h>

TEST_CASE("int formatter tests", "[formatter]") {
    char buffer[1024]{};

    SECTION("integer formatting - 1 byte")
    {
        SECTION("int8_t formatter") {
            strings::formatter intFormatter = strings::get_formatter(int8_t(16));
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
            CHECK(result == 2);
            REQUIRE("16" == std::string(buffer));
        }

        SECTION("int8_t formatter with custom format") {
            strings::formatter intFormatter = strings::get_formatter(int8_t(16));
            strings::format_options options{"%X"};
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]), &options);
            CHECK(result == 2);
            REQUIRE("10" == std::string(buffer));
        }

        SECTION("uint8_t formatter") {
            strings::formatter intFormatter = strings::get_formatter(uint8_t(16));
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
            CHECK(result == 2);
            REQUIRE("16" == std::string(buffer));
        }

        SECTION("uint8_t formatter with custom format") {
            strings::formatter intFormatter = strings::get_formatter(uint8_t(16));
            strings::format_options options{"%X"};
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]), &options);
            CHECK(result == 2);
            REQUIRE("10" == std::string(buffer));
        }
    }

    SECTION("integer formatting - 2 bytes")
    {
        SECTION("int16_t formatter") {
            strings::formatter intFormatter = strings::get_formatter(int16_t(16));
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
            CHECK(result == 2);
            REQUIRE("16" == std::string(buffer));
        }

        SECTION("int16_t formatter with custom format") {
            strings::formatter intFormatter = strings::get_formatter(int16_t(16));
            strings::format_options options{"%X"};
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]), &options);
            CHECK(result == 2);
            REQUIRE("10" == std::string(buffer));
        }

        SECTION("uint16_t formatter") {
            strings::formatter intFormatter = strings::get_formatter(uint16_t(16));
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
            CHECK(result == 2);
            REQUIRE("16" == std::string(buffer));
        }

        SECTION("uint16_t formatter with custom format") {
            strings::formatter intFormatter = strings::get_formatter(uint16_t(16));
            strings::format_options options{"%X"};
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]), &options);
            CHECK(result == 2);
            REQUIRE("10" == std::string(buffer));
        }
    }

    SECTION("integer formatting - 4 bytes")
    {
        SECTION("int32_t formatter") {
            strings::formatter intFormatter = strings::get_formatter(int32_t(16));
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
            CHECK(result == 2);
            REQUIRE("16" == std::string(buffer));
        }

        SECTION("int32_t formatter with custom format") {
            strings::formatter intFormatter = strings::get_formatter(int32_t(16));
            strings::format_options options{"%X"};
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]), &options);
            CHECK(result == 2);
            REQUIRE("10" == std::string(buffer));
        }

        SECTION("uint32_t formatter") {
            strings::formatter intFormatter = strings::get_formatter(uint32_t(16));
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
            CHECK(result == 2);
            REQUIRE("16" == std::string(buffer));
        }

        SECTION("uint32_t formatter with custom format") {
            strings::formatter intFormatter = strings::get_formatter(uint32_t(16));
            strings::format_options options{"%X"};
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]), &options);
            CHECK(result == 2);
            REQUIRE("10" == std::string(buffer));
        }
    }

    SECTION("integer formatting - 8 bytes")
    {
        SECTION("int64_t formatter") {
            strings::formatter intFormatter = strings::get_formatter(int64_t(16));
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
            CHECK(result == 2);
            REQUIRE("16" == std::string(buffer));
        }

        SECTION("int64_t formatter with custom format") {
            strings::formatter intFormatter = strings::get_formatter(int64_t(16));
            strings::format_options options{"%X"};
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]), &options);
            CHECK(result == 2);
            REQUIRE("10" == std::string(buffer));
        }

        SECTION("uint64_t formatter") {
            strings::formatter intFormatter = strings::get_formatter(uint64_t(16));
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
            CHECK(result == 2);
            REQUIRE("16" == std::string(buffer));
        }

        SECTION("uint64_t formatter with custom format") {
            strings::formatter intFormatter = strings::get_formatter(uint64_t(16));
            strings::format_options options{"%X"};
            size_t result = intFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]), &options);
            CHECK(result == 2);
            REQUIRE("10" == std::string(buffer));
        }
    }
}

TEST_CASE("bool formatter tests", "[formatter]") {
    char buffer[1024]{};
    SECTION("bool formatter") {
        strings::formatter boolFormatter = strings::get_formatter(true);
        size_t result = boolFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
        CHECK(result == 4);
        REQUIRE("true" == std::string(buffer));
    }
    SECTION("bool formatter with custom format") {
        strings::formatter boolFormatter = strings::get_formatter(true);
        strings::format_options options{"%X"};
        size_t result = boolFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]), &options);
        CHECK(result == 1);
        REQUIRE("1" == std::string(buffer));
    }
}

TEST_CASE("char* formatter tests", "[formatter]") {
    char buffer[1024]{};
    SECTION("char formatter") {
        strings::formatter charFormatter = strings::get_formatter('A');
        size_t result = charFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
        CHECK(result == 1);
        REQUIRE("A" == std::string(buffer));
    }
    SECTION("char formatter with custom format") {
        strings::formatter charFormatter = strings::get_formatter('A');
        strings::format_options options{"%d"};
        size_t result = charFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]), &options);
        CHECK(result == 2);
        REQUIRE("65" == std::string(buffer));
    }
    SECTION("wchar_t formatter") {
        strings::formatter wchar_t_Formatter = strings::get_formatter(L'A');
        size_t result = wchar_t_Formatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
        CHECK(result == 1);
        REQUIRE("A" == std::string(buffer));
    }
    SECTION("wchar_t formatter with custom format") {
        strings::formatter wchar_t_Formatter = strings::get_formatter(L'A');
        strings::format_options options{"%d"};
        size_t result = wchar_t_Formatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]), &options);
        CHECK(result == 2);
        REQUIRE("65" == std::string(buffer));
    }
    SECTION("char* formatter") {
        char source[] = "string";
        strings::formatter charPtrFormatter = strings::get_formatter(source);
        size_t result = charPtrFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
        CHECK(result == 6);
        REQUIRE("string" == std::string(buffer));
    }
    SECTION("wchar_t* formatter") {
        char expected[] = "unicode-\xD1\x80\xD1\x83\xD1\x81\xD1\x81\xD0\xBA\xD0\xB8\xD0\xB9"; // unicode-русский
        wchar_t source[] = L"unicode-русский";
        strings::formatter wchar_t_PtrFormatter = strings::get_formatter(source);
        size_t result = wchar_t_PtrFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
        CHECK(result == 22);
        REQUIRE(expected == std::string(buffer));
    }
}

TEST_CASE("std::string formatter tests", "[formatter]") {
    char buffer[1024]{};
    SECTION("std::string formatter") {
        std::string source = "string";
        strings::formatter stringFormatter = strings::get_formatter(source);
        size_t result = stringFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
        CHECK(result == 6);
        REQUIRE("string" == std::string(buffer));
    }
    SECTION("std::wstring formatter") {
        char expected[] = "unicode-\xD1\x80\xD1\x83\xD1\x81\xD1\x81\xD0\xBA\xD0\xB8\xD0\xB9"; // unicode-русский
        std::wstring source = L"unicode-русский";
        strings::formatter wstringFormatter = strings::get_formatter(source);
        size_t result = wstringFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
        CHECK(result == 22);
        REQUIRE(expected == std::string(buffer));
    }
}

TEST_CASE("pointer formatter tests", "[formatter]") {
    char buffer[1024]{};
    SECTION("pointer formatter") {
        void *source = (void*)0xDEADBEEF;
        strings::formatter stringFormatter = strings::get_formatter(source);
        size_t result = stringFormatter.format(buffer, sizeof(buffer) / sizeof(buffer[0]));
        bool DEADBEEF_match = std::string::npos != std::string(buffer).find("DEADBEEF");
        bool deadbeef_match = std::string::npos != std::string(buffer).find("deadbeef");
        REQUIRE((DEADBEEF_match || deadbeef_match));
    }
}