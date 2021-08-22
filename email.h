#ifndef EMAIL_H
#define EMAIL_H

#include <optional>
#include <string_view>
#include <string>
#include <cstdint>

class Email
{
    Email(std::string_view email, std::size_t midPos);
public:
    static bool Validate(std::string_view string);
    static std::optional<Email> TryParse(std::string_view string);

    std::string local() const;
    std::string_view domain() const;
    std::string_view fullAddress() const;
private:
    std::string m_email;
    std::size_t m_midPos;
};

#endif // EMAIL_H
