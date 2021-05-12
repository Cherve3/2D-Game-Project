
#include "bu_hazards.h"
#include "gfc_types.h"
#include "bu_items.h"

void new_hazard(TextWord *name, cpVect position)
{
	item_new(name, hazard, position);
}

/*eol@eof*/
