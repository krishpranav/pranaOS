/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "Card.h"
#include <AK/Format.h>
#include <AK/RefCounted.h>
#include <AK/Vector.h>

namespace Cards {

class CardStack final : public RefCounted<CardStack> {

public:
    enum Type {
        Invalid,
        Stock,
        
    }
}
}