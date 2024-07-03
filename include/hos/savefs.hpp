#pragma once

#include "es/es_Ticket.hpp"
#include "es/es_Cert.hpp"
#include "Crypto.hpp"
#include <switch.h>


namespace Mara::hos {
    es::TicketFile ReadTicket(u64 app_id);
    void ReadCert(const char* issuer);
}