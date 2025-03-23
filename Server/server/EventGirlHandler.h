#pragma once


class CEventGirlHandler
{

public:
				CEventGirlHandler();
	virtual		~CEventGirlHandler();

	void		OnEventGirlResetRequest(User* pcUser, struct PacketEventGirlReset* psPacket);
};
