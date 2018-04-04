#include "pch.h"
#include "AdaptiveCard.h"
#include "AdaptiveShowCardAction.h"
#include "Util.h"

using namespace Microsoft::WRL;
using namespace ABI::AdaptiveNamespace;

AdaptiveNamespaceStart
    HRESULT AdaptiveShowCardAction::RuntimeClassInitialize() noexcept try
    {
        std::shared_ptr<AdaptiveSharedNamespace::ShowCardAction> showCardAction = std::make_shared<AdaptiveSharedNamespace::ShowCardAction>();
        return RuntimeClassInitialize(showCardAction);
    } CATCH_RETURN;

    _Use_decl_annotations_
    HRESULT AdaptiveShowCardAction::RuntimeClassInitialize(const std::shared_ptr<AdaptiveSharedNamespace::ShowCardAction>& sharedShowCardAction)
    {
        if (sharedShowCardAction == nullptr)
        {
            return E_INVALIDARG;
        }

        RETURN_IF_FAILED(MakeAndInitialize<AdaptiveCard>(&m_card, sharedShowCardAction->GetCard()));
        RETURN_IF_FAILED(UTF8ToHString(sharedShowCardAction->GetId(), m_id.GetAddressOf()));
        RETURN_IF_FAILED(UTF8ToHString(sharedShowCardAction->GetTitle(), m_title.GetAddressOf()));
        RETURN_IF_FAILED(JsonCppToJsonObject(sharedShowCardAction->GetAdditionalProperties(), &m_additionalProperties));

        return S_OK;
    }

    IFACEMETHODIMP AdaptiveShowCardAction::get_Card(ABI::AdaptiveNamespace::IAdaptiveCard** card)
    {
        return m_card.CopyTo(card);
    }

    IFACEMETHODIMP AdaptiveShowCardAction::put_Card(ABI::AdaptiveNamespace::IAdaptiveCard* card)
    {
        m_card = card;
        return S_OK;
    }

    _Use_decl_annotations_
    HRESULT AdaptiveShowCardAction::get_Title(HSTRING* title)
    {
        return m_title.CopyTo(title);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveShowCardAction::put_Title(HSTRING title)
    {
        return m_title.Set(title);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveShowCardAction::get_ActionType(ABI::AdaptiveNamespace::ActionType* actionType)
    {
        *actionType = ABI::AdaptiveNamespace::ActionType::ShowCard;
        return S_OK;
    }

    _Use_decl_annotations_
    HRESULT AdaptiveShowCardAction::get_Id(HSTRING* id)
    {
        return m_id.CopyTo(id);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveShowCardAction::put_Id(HSTRING id)
    {
        return m_id.Set(id);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveShowCardAction::get_ActionTypeString(HSTRING* type)
    {
        ::ActionType typeEnum;
        RETURN_IF_FAILED(get_ActionType(&typeEnum));
        return ProjectedActionTypeToHString(typeEnum, type);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveShowCardAction::get_AdditionalProperties(ABI::Windows::Data::Json::IJsonObject** result)
    {
        return m_additionalProperties.CopyTo(result);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveShowCardAction::put_AdditionalProperties(ABI::Windows::Data::Json::IJsonObject* jsonObject)
    {
        m_additionalProperties = jsonObject;
        return S_OK;
    }

    _Use_decl_annotations_
    HRESULT AdaptiveShowCardAction::ToJson(ABI::Windows::Data::Json::IJsonObject** result)
    {
        std::shared_ptr<AdaptiveSharedNamespace::ShowCardAction> sharedModel;
        RETURN_IF_FAILED(GetSharedModel(sharedModel));

        return StringToJsonObject(sharedModel->Serialize(), result);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveShowCardAction::GetSharedModel(std::shared_ptr<AdaptiveSharedNamespace::ShowCardAction>& sharedModel)
    {
        std::shared_ptr<AdaptiveSharedNamespace::ShowCardAction> showCardAction = std::make_shared<AdaptiveSharedNamespace::ShowCardAction>();

        showCardAction->SetId(HStringToUTF8(m_id.Get()));
        showCardAction->SetTitle(HStringToUTF8(m_title.Get()));

        Json::Value jsonValue;
        JsonObjectToJsonCpp(m_additionalProperties.Get(), &jsonValue);
        showCardAction->SetAdditionalProperties(jsonValue);

        ComPtr<AdaptiveNamespace::AdaptiveCard> card = PeekInnards<AdaptiveNamespace::AdaptiveCard>(m_card);
        
        std::shared_ptr<AdaptiveSharedNamespace::AdaptiveCard> sharedCard;
        RETURN_IF_FAILED(card->GetSharedModel(sharedCard));

        showCardAction->SetCard(sharedCard);
        
        sharedModel = showCardAction;
        return S_OK;
    }
AdaptiveNamespaceEnd
