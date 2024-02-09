// Copyright Rive, Inc. All rights reserved.

#include "RiveFile_AssetDefinitionDefault.h"

#include "ContentBrowserMenuContexts.h"
#include "IAssetTools.h"
#include "RiveAssetEditor.h"
#include "Factories/RiveFileInstanceFactory.h"
#include "Logs/RiveEditorLog.h"
#include "Rive/RiveFile.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

namespace MenuExtension_RiveFile
{
	void ExecuteCreateInstance(const FToolMenuContext& InContext)
	{
		UE_LOG(LogRiveEditor, Log, TEXT("Test!!!"));
		const UContentBrowserAssetContextMenuContext* CBContext = UContentBrowserAssetContextMenuContext::FindContextWithAssets(InContext);

		TArray<URiveFile*> x = CBContext->LoadSelectedObjects<URiveFile>();

		IAssetTools::Get().CreateAssetsFrom<URiveFile>(CBContext->LoadSelectedObjects<URiveFile>(), URiveFile::StaticClass(), TEXT("_Inst"), [](URiveFile* SourceObject)
		{
			UE_LOG(LogRiveEditor, Log, TEXT("Instancing Rive File: %s"), *SourceObject->GetName());
			URiveFileInstanceFactory* Factory = NewObject<URiveFileInstanceFactory>();
			Factory->InitialRiveFile = SourceObject;
			return Factory;
			
		});
	}

	static FDelayedAutoRegisterHelper DelayedAutoRegister(EDelayedRegisterRunPhase::EndOfEngineInit, []{ 
		UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateLambda([]()
		{
			FToolMenuOwnerScoped OwnerScoped(UE_MODULE_NAME);
			UToolMenu* Menu = UE::ContentBrowser::ExtendToolMenu_AssetContextMenu(URiveFile::StaticClass());
		
			FToolMenuSection& Section = Menu->FindOrAddSection("GetAssetActions");
				Section.AddDynamicEntry(NAME_None, FNewToolMenuSectionDelegate::CreateLambda([](FToolMenuSection& InSection)
				{
					if (const UContentBrowserAssetContextMenuContext* Context = UContentBrowserAssetContextMenuContext::FindContextWithAssets(InSection))
					{
						{
							const TAttribute<FText> Label = LOCTEXT("RiveFile_CreateInstance", "Create Rive Instance");
							const TAttribute<FText> ToolTip = LOCTEXT("RiveFile_CreateInstanceTooltip", "Creates a new Rive instance using this file.");
							const FSlateIcon Icon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "ClassIcon.Material");
							const FToolMenuExecuteAction UIAction = FToolMenuExecuteAction::CreateStatic(&ExecuteCreateInstance);
							InSection.AddMenuEntry("RiveFile_CreateInstance", Label, ToolTip, Icon, UIAction);
						}
					}
				}));
		}));
	});
}

FText URiveFile_AssetDefinitionDefault::GetAssetDisplayName() const
{
	return LOCTEXT("AssetTypeActions_SmartObjectDefinition", "SmartObject Definition");
}

FLinearColor URiveFile_AssetDefinitionDefault::GetAssetColor() const
{
	return FLinearColor::Red;
}

TSoftClassPtr<> URiveFile_AssetDefinitionDefault::GetAssetClass() const
{
	return URiveFile::StaticClass();
}

TConstArrayView<FAssetCategoryPath> URiveFile_AssetDefinitionDefault::GetAssetCategories() const
{
	static const FAssetCategoryPath Categories[] = { EAssetCategoryPaths::Misc };

	return Categories;
}

EAssetCommandResult URiveFile_AssetDefinitionDefault::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();

	for (URiveFile* Definition : OpenArgs.LoadObjects<URiveFile>())
	{
		URiveAssetEditor* AssetEditor = NewObject<URiveAssetEditor>(AssetEditorSubsystem, NAME_None, RF_Transient);
		
		AssetEditor->SetObjectToEdit(Definition);
		
		AssetEditor->Initialize();
	}

	return EAssetCommandResult::Handled;
}

#undef LOCTEXT_NAMESPACE
