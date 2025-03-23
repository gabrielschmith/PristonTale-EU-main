#pragma once
class CDamageHandler
{
public:
	CDamageHandler();
	virtual ~CDamageHandler();

	//JLM - TODO
	//std::vector<PacketDamageQuickDelayed*>				delayedDamageList;

	void												HandlePacket( struct PacketDamageQuickDelayed * psPacket );
	void												HandlePacket( struct PacketDamageQuick * psPacket );
};

