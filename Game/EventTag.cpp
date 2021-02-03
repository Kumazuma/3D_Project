#include "EventTag.hpp"
namespace Kumazuma::Client
{
	Game::EventTag<DamageEvent> const EVT_Damage{ "EVT_Damage" };
	Game::EventTag<BuffEvent> const EVT_Buff{"EVT_Buff"};
}