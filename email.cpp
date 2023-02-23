#include "email.h"

constexpr bool IsAlhaNumericChar(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9');
}
constexpr bool IsValidLocalChar(char  c)
{
    return IsAlhaNumericChar(c) ||
           c == '!'  ||
           c == '#'  ||
           c == '$'  ||
           c == '%'  ||
           c == '&'  ||
           c == '*'  ||
           c == '+'  ||
           c == '/'  ||
           c == '='  ||
           c == '?'  ||
           c == '^'  ||
           c == '_'  ||
           c == '`'  ||
           c == '{'  ||
           c == '|'  ||
           c == '}'  ||
           c == '~'  ||
           c == '-';
}
constexpr bool IsValidLocalPart(const char*& it)
{
    const char* startIt = it;
    do {
        if (!IsValidLocalChar(*it++)) {
            return false;
        }

        while (IsValidLocalChar(*it)) { it++; }
    }
    while (*it++ == '.');

    int localLen = (it - 1) - startIt;

    return *(it - 1) == '@' && localLen <= 64;
}
constexpr bool IsValidDomainPart(const char*& it)
{
    char c = 0;
    do {
        const char* startIt = it;
        while (IsAlhaNumericChar(*it)) { it++; }

        int dnsLabelLen = it - startIt;
        if (dnsLabelLen < 1 || dnsLabelLen > 63) {
            return false;
        }

        c = *it++;
    }
    while (c == '-' || c == '.');

    it--;

    return c == 0;
}
constexpr std::size_t IsValidEmail(std::string_view email)
{
    const char* begin = email.data();
    const char* it = begin;

    // Check email size
    if (email.length() > 254 || email.length() < 3) {
        return 0;
    }

    // Verify that email recepient section is valid
    if (!IsValidLocalPart(it)) {
        return 0;
    }

    std::size_t midPart = (it - 1) - begin;

    // Validate domain validity
    if (!IsValidDomainPart(it)) {
        return 0;
    }

    // Ensure that the entire email has been checked
    std::size_t validatedLength = it - begin;
    if (validatedLength != email.length()) {
        return 0;
    }

    return midPart;
}

Email::Email(std::string_view email, size_t midPos)
    : m_email(email)
    , m_midPos(midPos)
{
}

bool Email::Validate(std::string_view string)
{
    return IsValidEmail(string) != 0;
}

std::optional<Email> Email::TryParse(std::string_view string)
{
    std::size_t midPos = IsValidEmail(string);

    if (midPos == 0) {
        return {};
    }

    return Email(string, midPos);
}

std::string Email::local() const
{
    return std::string(m_email.begin(), m_email.begin() + m_midPos);
}

std::string_view Email::domain() const
{
    return std::string_view(m_email.begin() + m_midPos + 1, m_email.end());
}

std::string_view Email::fullAddress() const
{
    return m_email;
}
