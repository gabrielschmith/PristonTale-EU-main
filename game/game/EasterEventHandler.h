#pragma once
class CEasterEventHandler
{
public:
	CEasterEventHandler();
	virtual ~CEasterEventHandler();


	void									HandlePacket( struct PacketNPCEasterEgg * psPacket );

	BOOL									IsValidRune(class ItemData* pcItem);

	void									OnClickOKNPC();

protected:

	void									OnOKBoxClick(UIEventArgs e);
};

