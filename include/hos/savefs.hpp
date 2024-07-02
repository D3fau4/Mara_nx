#pragma once

#include "es/es.hpp"
#include "Crypto.hpp"

namespace Mara::hos {
    es::TicketFile ReadTicket(const u64 app_id);
}