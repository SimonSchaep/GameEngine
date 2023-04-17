#pragma once
#include "Event.h"

//it's probably not ideal

class DieEvent : public Event
{
};

class ResetLivesEvent : public Event
{
};

class KilledEnemyEvent : public Event
{
};