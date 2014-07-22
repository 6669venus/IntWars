/*
IntWars playground server for League of Legends protocol testing
Copyright (C) 2012  Intline9 <Intline9@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#if defined(WIN32) || defined(_WIN32)
   #include "win/platforms-time.h"
#endif

#include <sys/time.h>
#include "stdafx.h"
#include "Game.h"

#define REFRESH_RATE 16

uint32 GetNewNetID() {
	static uint32 dwStart = 0x40000019;
	uint32 dwRet = dwStart;
	dwStart++;
	return dwRet;
}

Game::Game()
{

}

Game::~Game()
{
	_isAlive = false;

	delete _blowfish;
	enet_host_destroy(_server);
}

bool Game::initialize(ENetAddress *address, const char *baseKey)
{
	if (enet_initialize () != 0)
		return false;
	atexit(enet_deinitialize);

	_server = enet_host_create(address, 32, 0, 0);
	if(_server == NULL)
		return false;

	std::string key = base64_decode(baseKey);
	if(key.length() <= 0)
		return false;
      
	_blowfish = new BlowFish((uint8*)key.c_str(), 16);
	initHandlers();
   
   map = new Map();
	
	return _isAlive = true;
}

void Game::netLoop()
{
	ENetEvent event;
   struct timeval tStart, tEnd, tDiff;

	while(true)
	{
      gettimeofday(&tStart, 0);
      while(enet_host_service(_server, & event, 1) > 0) {
         switch (event.type)
         {
         case ENET_EVENT_TYPE_CONNECT:
            //Logging->writeLine("A new client connected: %i.%i.%i.%i:%i \n", event.peer->address.host & 0xFF, (event.peer->address.host >> 8) & 0xFF, (event.peer->address.host >> 16) & 0xFF, (event.peer->address.host >> 24) & 0xFF, event.peer->address.port);

            /* Set some defaults */
            event.peer->mtu = PEER_MTU;

            event.peer->data = new ClientInfo();
            peerInfo(event.peer)->setName("Test");
            peerInfo(event.peer)->setChampion(ChampionFactory::getChampionFromType("Ezreal", map, GetNewNetID()));
            peerInfo(event.peer)->skinNo = 6;

            break;

         case ENET_EVENT_TYPE_RECEIVE:
            currentPeer = event.peer;
            if(!handlePacket(event.peer, event.packet,event.channelID))
            {
               //enet_peer_disconnect(event.peer, 0);
            }

            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy (event.packet);
            break;

         case ENET_EVENT_TYPE_DISCONNECT:
            delete (ClientInfo*)event.peer->data;
            break;
         }
      }
      map->update(REFRESH_RATE);
      gettimeofday(&tEnd, 0);
      timersub(&tEnd, &tStart, &tDiff);
      usleep(REFRESH_RATE*1000 - (tDiff.tv_sec*1000000+tDiff.tv_usec));
   }
}