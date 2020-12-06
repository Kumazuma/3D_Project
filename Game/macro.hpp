#pragma once
#define DECLARE_COM_TAG(type) public: static const Kumazuma::Game::ComponentTag<type> TAG;
#define IMPLEMENT_COM_TAG(type) const Kumazuma::Game::ComponentTag<type> type::TAG{#type};