#include "pch.h"
#include "AdaptiveColumnSet.h"

#include "Util.h"
#include "Vector.h"
#include <windows.foundation.collections.h>
#include "XamlHelpers.h"
#include "AdaptiveColumn.h"

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::AdaptiveNamespace;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::UI::Xaml;
using namespace ABI::Windows::UI::Xaml::Controls;

AdaptiveNamespaceStart
    AdaptiveColumnSet::AdaptiveColumnSet()
    {
        m_columns = Microsoft::WRL::Make<Vector<IAdaptiveColumn*>>();
    }

    HRESULT AdaptiveColumnSet::RuntimeClassInitialize() noexcept try
    {
        std::shared_ptr<AdaptiveSharedNamespace::ColumnSet> columnSet = std::make_shared<AdaptiveSharedNamespace::ColumnSet>();
        return RuntimeClassInitialize(columnSet);
    } CATCH_RETURN;

    _Use_decl_annotations_
    HRESULT AdaptiveColumnSet::RuntimeClassInitialize(const std::shared_ptr<AdaptiveSharedNamespace::ColumnSet>& sharedColumnSet)
    {
        if (sharedColumnSet == nullptr)
        {
            return E_INVALIDARG;
        }

        GenerateColumnsProjection(sharedColumnSet->GetColumns(), m_columns.Get());
        GenerateActionProjection(sharedColumnSet->GetSelectAction(), &m_selectAction);

        m_spacing = static_cast<ABI::AdaptiveNamespace::Spacing>(sharedColumnSet->GetSpacing());
        m_separator = sharedColumnSet->GetSeparator();
        RETURN_IF_FAILED(UTF8ToHString(sharedColumnSet->GetId(), m_id.GetAddressOf()));
        RETURN_IF_FAILED(JsonCppToJsonObject(sharedColumnSet->GetAdditionalProperties(), &m_additionalProperties));

        return S_OK;
    }

    _Use_decl_annotations_
    IFACEMETHODIMP AdaptiveColumnSet::get_Columns(IVector<IAdaptiveColumn*>** columns)
    {
        return m_columns.CopyTo(columns);
    }

    _Use_decl_annotations_
    IFACEMETHODIMP AdaptiveColumnSet::get_SelectAction(IAdaptiveActionElement** action)
    {
        return m_selectAction.CopyTo(action);
    }

    _Use_decl_annotations_
    IFACEMETHODIMP AdaptiveColumnSet::put_SelectAction(IAdaptiveActionElement* action)
    {
        m_selectAction = action;
        return S_OK;
    }

    _Use_decl_annotations_
    IFACEMETHODIMP AdaptiveColumnSet::get_ElementType(ElementType* elementType)
    {
        *elementType = ElementType::ColumnSet;
        return S_OK;
    }

    _Use_decl_annotations_
    HRESULT AdaptiveColumnSet::get_Spacing(ABI::AdaptiveNamespace::Spacing* spacing)
    {
        *spacing = m_spacing;
        return S_OK;
    }

    _Use_decl_annotations_
    HRESULT AdaptiveColumnSet::put_Spacing(ABI::AdaptiveNamespace::Spacing spacing)
    {
        m_spacing = spacing;
        return S_OK;
    }

    _Use_decl_annotations_
    HRESULT AdaptiveColumnSet::get_Separator(boolean* separator)
    {
        *separator = m_separator;
        return S_OK;
    }

    _Use_decl_annotations_
    HRESULT AdaptiveColumnSet::put_Separator(boolean separator)
    {
        m_separator = separator;
        return S_OK;
    }

    _Use_decl_annotations_
    HRESULT AdaptiveColumnSet::get_Id(HSTRING* id)
    {
        return m_id.CopyTo(id);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveColumnSet::put_Id(HSTRING id)
    {
        return m_id.Set(id);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveColumnSet::get_ElementTypeString(HSTRING* type)
    {
        ElementType typeEnum;
        RETURN_IF_FAILED(get_ElementType(&typeEnum));
        return ProjectedElementTypeToHString(typeEnum, type);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveColumnSet::get_AdditionalProperties(ABI::Windows::Data::Json::IJsonObject** result)
    {
        return m_additionalProperties.CopyTo(result);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveColumnSet::put_AdditionalProperties(ABI::Windows::Data::Json::IJsonObject* jsonObject)
    {
        m_additionalProperties = jsonObject;
        return S_OK;
    }

    _Use_decl_annotations_
    HRESULT AdaptiveColumnSet::ToJson(ABI::Windows::Data::Json::IJsonObject** result)
    {
        std::shared_ptr<AdaptiveSharedNamespace::ColumnSet> sharedModel;
        RETURN_IF_FAILED(GetSharedModel(sharedModel));

        return StringToJsonObject(sharedModel->Serialize(), result);
    }

    _Use_decl_annotations_
    HRESULT AdaptiveColumnSet::GetSharedModel(std::shared_ptr<AdaptiveSharedNamespace::ColumnSet>& sharedModel) try
    {
        std::shared_ptr<AdaptiveSharedNamespace::ColumnSet> columnSet = std::make_shared<AdaptiveSharedNamespace::ColumnSet>();

        RETURN_IF_FAILED(SetSharedElementProperties(this, std::AdaptivePointerCast<AdaptiveSharedNamespace::BaseCardElement>(columnSet)));

        if (m_selectAction != nullptr)
        {
            std::shared_ptr<BaseActionElement> sharedAction;
            RETURN_IF_FAILED(GenerateSharedAction(m_selectAction.Get(), sharedAction));
            columnSet->SetSelectAction(sharedAction);
        }

        XamlHelpers::IterateOverVector<IAdaptiveColumn>(m_columns.Get(), [&](IAdaptiveColumn* column)
        {
            std::shared_ptr<Column> sharedColumn = std::make_shared<Column>();
            ComPtr<AdaptiveNamespace::AdaptiveColumn> columnImpl = PeekInnards<AdaptiveNamespace::AdaptiveColumn>(column);
            RETURN_IF_FAILED(columnImpl->GetSharedModel(sharedColumn));

            columnSet->GetColumns().push_back(sharedColumn);

            return S_OK;
        });

        sharedModel = columnSet;
        return S_OK;
    }CATCH_RETURN;
AdaptiveNamespaceEnd
