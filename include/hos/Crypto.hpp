#pragma once
#include <string>
#include <vector>
#include <mbedtls/rsa.h>
#include <mbedtls/sha1.h>
#include <mbedtls/sha256.h>
#include "es/es_Ticket.hpp"
#include "es/es_Cert.hpp"

namespace Mara::hos::Crypto {
    bool CheckTicket(Mara::es::TicketFile tik, Mara::es::Cert cert);
}