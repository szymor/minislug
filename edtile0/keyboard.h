
// Struct pour le clavier.
struct SKeyboard
{
	u8	*pKeys;		// Buffer clavier SDL.

};

struct SKeyboard * Kbd_New(void);
void Kbd_Delete(struct SKeyboard *psKbd);

void Kbd_Manage(struct SKeyboard *psKbd);
u32 Kbd_MstInfoBoxKbManage(struct SKeyboard *psKbd);


enum
{
	e_KB_MST_Null = 0,
	e_KB_MST_Ok,
	e_KB_MST_Cancel,
};

