//
// Created by justme on 13.12.16.
//

#ifndef UPSSERVER_PACKETFACTORY_H
#define UPSSERVER_PACKETFACTORY_H


class PacketFactory {
public:

    static PacketFactory *instance();

    void init();
private:
    static PacketFactory *INSTANCE;

    PacketFactory();

};


#endif //UPSSERVER_PACKETFACTORU_H
