//
// Created by qc on 24-10-20.
//

#include "Poller.h"


Poller::Poller(EventLoop *loop)
        : ownerLoop_(loop)
{

}

bool Poller::hasChannel(Channel *channel) const
{
    auto it = channels_.find(channel->fd());
    return it != channels_.end() && it->second == channel;
}

