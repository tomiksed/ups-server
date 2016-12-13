//
// Created by justme on 13.12.16.
//

#include "PacketFactory.h"

PacketFactory *PacketFactory::INSTANCE = new PacketFactory();

PacketFactory *PacketFactory::instance() {
    return PacketFactory::INSTANCE;
}

PacketFactory::PacketFactory() {}
