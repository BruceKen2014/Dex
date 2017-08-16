

#include "DexEngineCommandPanel.h"
#include "../widget/DexWidgetEditBox.h"
#include "../widget/DexWidgetFactory.h"
#include "../Expression/DexExpression.h"
#include "DexLog.h"
#include "../state/DexGameEngine.h"
#include "DexCommonFunction.h"
DexPanelCommand::DexPanelCommand()
{
	m_commandIndex = 0;
	m_pEditBox = DexNull;	
}
DexPanelCommand::~DexPanelCommand()
{
	m_pEditBox = DexNull;
	getWidgetFactory()->removeWidget(m_pEditBox->GetName());
}

bool DexPanelCommand::Initialize()
{
	m_pEditBox = (DexWidgetEditBox*)getWidgetFactory()->createWidget(widget_editbox, "command input");
	m_pEditBox->Resize(600, 15);
	m_pEditBox->SetPos(0.0f, DexGameEngine::getEngine()->GetWindowHeight() - m_pEditBox->GetSize().y - 40); //再减去大致菜单栏的高度
	m_pEditBox->OpenMask(true);
	m_pEditBox->SetMaskColor(DEXCOLOR_PURPLE);
	m_pEditBox->SetMaskAlpha(80);
	m_pEditBox->SetTextSize(15);
	m_pEditBox->SetTextColor(DEXCOLOR_WHITE);
	m_pEditBox->ModifyFlag(Add_Flag, DexGUI::mouse_move);
	m_pEditBox->m_DisTextUpdate += UI_DELEGATE(this, DexPanelCommand, OnUpdateCommand);
	m_pEditBox->m_DisMiscKey += UI_DELEGATE(this, DexPanelCommand, OnEditBoxMiscKey);
	setVisible(false);
	return true;
}

void DexPanelCommand::setVisible(bool visible)
{
	DEX_ENSURE(m_pEditBox != DexNull);
	m_pEditBox->setVisible(visible);
	m_pEditBox->SetSelect(m_pEditBox->GetVisible());
}

DBool DexPanelCommand::getVisible()
{
	DEX_ENSURE_B(m_pEditBox != DexNull);
	return m_pEditBox->GetVisible();
}

DBool DexPanelCommand::Update(int delta)
{
	DEX_ENSURE_B(Super::Update(delta));
	DEX_ENSURE_B(m_pEditBox->Update(delta));
	return DTRUE;
}

void DexPanelCommand::OnTimer(const stTimer& timer)
{
}

DBool DexPanelCommand::Render()
{
	m_pEditBox->Render();
	return DTRUE;
}

void DexPanelCommand::OnEditBoxMiscKey(CEventHandler* sys, DexObject* object, stEvent event)
{
	DexPanelCommand* system = (DexPanelCommand*)sys;
	stArgs args = event.back_args();
	if (args.i_member == EVK_UP)
	{
		if (system->m_historyCommands.size() != 0)
		{
			DexCommonFunction::clip(--system->m_commandIndex, (DInt8)0, (DInt8)(system->m_historyCommands.size() - 1));
			system->m_pEditBox->SetText(system->m_historyCommands[system->m_commandIndex]);
		}
	}
	else if (args.i_member == EVK_DOWN)
	{
		if (system->m_historyCommands.size() != 0)
		{
			DexCommonFunction::clip(++system->m_commandIndex, (DInt8)0, (DInt8)(system->m_historyCommands.size()-1));
			system->m_pEditBox->SetText(system->m_historyCommands[system->m_commandIndex]);
		}
	}
}
void DexPanelCommand::OnUpdateCommand(CEventHandler* sys, DexObject* object, stEvent event)
{
	DexPanelCommand* system = (DexPanelCommand*)sys;
	DexLog::getSingleton()->LogLine(log_ok, (char*)(system->m_pEditBox->GetEditText()).c_str());

	DexGameEngine::getEngine()->ExecCommand(system->m_pEditBox->GetEditText());
	if (system->m_historyCommands.size() == 0)
	{//命令不同再更新
		system->m_historyCommands.push_back(system->m_pEditBox->GetEditText());
		system->m_commandIndex = system->m_historyCommands.size() - 1;
	}
	else if (system->m_historyCommands[system->m_historyCommands.size() - 1] != system->m_pEditBox->GetEditText())
	{
		system->m_historyCommands.push_back(system->m_pEditBox->GetEditText());
		system->m_commandIndex = system->m_historyCommands.size() - 1;
	}
	system->m_pEditBox->ClearText();
}