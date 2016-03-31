#pragma once
#include <bws/packer/Client.hpp>

#include <iostream>
#include <samchon/library/XML.hpp>
#include <samchon/protocol/Invoke.hpp>

#include <bws/packer/PackerForm.hpp>
#include <bws/packer/Packer.hpp>
#include <bws/packer/WrapperArray.hpp>
#include <samchon/library/GAParameters.hpp>

#include <samchon/library/ProgressEvent.hpp>

using namespace std;
using namespace bws::packer;
using namespace samchon::library;
using namespace samchon::protocol;

void handleProgress(shared_ptr<Event> , void *);

Client::Client(Socket *socket)
	: super()
{
	this->socket = socket;
}

void Client::_replyData(shared_ptr<Invoke> invoke)
{
	replyData(invoke);

	//try
	//{
	//	replyData(invoke);
	//}
	//catch (exception &e)
	//{
	//	cout << "exception: " << e.what() << endl;
	//}
	//catch (...)
	//{
	//	cout << "unknown error" << endl;
	//}
}
void Client::replyData(shared_ptr<Invoke> invoke)
{
	cout << invoke->getListener() << endl;

	if (invoke->getListener() == "pack")
		pack(invoke->at(0)->getValueAsXML());
}

void Client::pack(shared_ptr<XML> xml)
{
	unique_ptr<PackerForm> packerForm(new PackerForm());
	packerForm->construct(xml);

	auto packer = packerForm->toPacker();
	auto gaParams = packerForm->getGAParameters();

	packer->addEventListener(ProgressEvent::PROGRESS, handleProgress, this);

	shared_ptr<WrapperArray> wrapperArray = packer->optimize(*gaParams);
	
	sendData(make_shared<Invoke>("setWrapperArray", wrapperArray->toXML()));
}

void handleProgress(shared_ptr<Event> evt, void *lpVoid)
{
	Client *client = (Client*)lpVoid;
	ProgressEvent *event = (ProgressEvent*)evt.get();

	client->sendData(make_shared<Invoke>("setProgress", event->getNumerator(), event->getDenominator()));
}