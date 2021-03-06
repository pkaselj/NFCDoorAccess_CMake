/*
*	 Copyright (C) Petar Kaselj 2021
*
*	 This file is part of NFCDoorAccess.
*
*	 NFCDoorAccess is written by Petar Kaselj as an employee of
*	 Emovis tehnologije d.o.o. which allowed its release under
*	 this license.
*
*    NFCDoorAccess is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    NFCDoorAccess is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with NFCDoorAccess.  If not, see <https://www.gnu.org/licenses/>.
*
*/

#ifndef AUTOMATON_PAIR_FACTORY
#define AUTOMATON_PAIR_FACTORY

#include "MainAutomaton.hpp"
#include "KeypadAutomaton.hpp"

/**
  * Class used to instantiate and pass resources to automata.
  * Used to hide cyclic dependecies between automata.
**/
class AutomatonPairFactory
{
public:
	AutomatonPairFactory(DataMailbox* pMailbox,
		MailboxReference* pRefDatabase,
		IndicatorController_Client* pIndicators,
		ILogger* pLogger = NulLogger::getInstance());

	~AutomatonPairFactory();

	MainAutomaton& getMainAutomatonReference() const;
	KeypadAutomaton& getKeypadAutomatonReference() const;


private:

	ILogger* m_pLogger;
	DataMailbox* m_pMailbox;
	MailboxReference* m_pRefDatabase;
	IndicatorController_Client* m_pIndicators;

	MainAutomaton* m_pMainAutomaton;
	KeypadAutomaton* m_pKeypadAutomaton;

	void createMainAutomaton();
	void createKeypadAutomaton();

	/// Pass reference of one automaton to others and vice versa
	void createAutomataCoupling();

	void initializeAutomata();

};

AutomatonPairFactory::AutomatonPairFactory(DataMailbox* pMailbox,
	MailboxReference* pRefDatabase,
	IndicatorController_Client* pIndicators,
	ILogger* pLogger)
	:	m_pMailbox(pMailbox),
	m_pLogger(pLogger),
	m_pRefDatabase(pRefDatabase),
	m_pIndicators(pIndicators),
	m_pKeypadAutomaton(nullptr),
	m_pMainAutomaton(nullptr)
{
	if (m_pLogger == nullptr)
	{
		m_pLogger = NulLogger::getInstance();
	}

	if (m_pMailbox == nullptr)
	{
		*m_pLogger << "AutomatonPairFactory - Mailbox pointer cannot be null!";
		Kernel::Fatal_Error("AutomatonPairFactory - Mailbox pointer cannot be null!");
	}

	if (m_pRefDatabase == nullptr)
	{
		*m_pLogger << "AutomatonPairFactory - Mailbox reference to database cannot be null!";
		Kernel::Fatal_Error("AutomatonPairFactory - Mailbox reference to database cannot be null!");
	}

	if (m_pIndicators == nullptr)
	{
		*m_pLogger << "AutomatonPairFactory - Indicator Controller cannot be null!";
		Kernel::Fatal_Error("AutomatonPairFactory - Indicator Controller cannot be null!");
	}

	createMainAutomaton();
	createKeypadAutomaton();

	createAutomataCoupling();

	initializeAutomata();
}

AutomatonPairFactory::~AutomatonPairFactory()
{
	delete m_pMainAutomaton;
	delete m_pKeypadAutomaton;
}

void AutomatonPairFactory::createMainAutomaton()
{
	m_pMainAutomaton = new MainAutomaton(0, m_pIndicators, m_pLogger);
}

void AutomatonPairFactory::createKeypadAutomaton()
{
	m_pKeypadAutomaton = new KeypadAutomaton(1, m_pMailbox, m_pRefDatabase, m_pIndicators, m_pLogger);
}

void AutomatonPairFactory::createAutomataCoupling()
{
	m_pKeypadAutomaton->setMainAutomatonPointer(m_pMainAutomaton);
	m_pMainAutomaton->setKeypadAutomatonPointer(m_pKeypadAutomaton);
}

void AutomatonPairFactory::initializeAutomata()
{
	if (m_pMainAutomaton->initialize() == false)
	{
		*m_pLogger << "Main automaton could not be initialized.";
		Kernel::Fatal_Error("Main automaton could not be initialized.");
	}

	if (m_pKeypadAutomaton->initialize() == false)
	{
		*m_pLogger << "Keypad automaton could not be initialized.";
		Kernel::Fatal_Error("Keypad automaton could not be initialized.");
	}

}

MainAutomaton& AutomatonPairFactory::getMainAutomatonReference() const
{
	return *m_pMainAutomaton;
}

KeypadAutomaton& AutomatonPairFactory::getKeypadAutomatonReference() const
{
	return *m_pKeypadAutomaton;
}


#endif