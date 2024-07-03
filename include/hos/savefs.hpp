#pragma once

#include "es/es_Ticket.hpp"
#include "Crypto.hpp"

namespace Mara::hos {
    es::TicketFile ReadTicket(const u64 app_id);
}