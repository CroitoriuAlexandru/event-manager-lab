#pragma once
#include <Wt/WFormModel.h>
#include <Wt/WTemplateFormView.h>
#include <Wt/WValidator.h>
#include "EventDataModule.h" // for passing data from form

// Event Form model
class EventFormModel : public Wt::WFormModel
{
public:
    // Associate each field with a unique string literal.
    static const Field ClientNameField;
    static const Field ClientPhoneNumberField;
    static const Field EventDateField;
    static const Field EventStartField;
    static const Field EventDurationField;
    static const Field EventLocationField;
    static const Field EventObservationsField;

    EventFormModel();

    // Get model data
    EventDataModule::StructRegisterEvent getData();

private:
    // Name Validator
    std::shared_ptr<Wt::WValidator> createTextValidator(bool mandatory, int min, int max);

    // Date Validator
    std::shared_ptr<Wt::WValidator> createDateValidator();

    // Event Start Time Validator
    std::shared_ptr<Wt::WValidator> createEventStartTimeValidator();

    // Event Duration Validator
    std::shared_ptr<Wt::WValidator> createEventDurationValidator(double min, double max);
};

// Event Form View
class EventFormView : public Wt::WTemplateFormView
{
public:
    EventFormView();

    std::shared_ptr<EventFormModel> model_;

private:
    void process();
};

// Service Form Model
class ServiceFormModel : public Wt::WFormModel
{
public:
    // Associate each field with a unique string literal.
    static const Field ServiceProviderIdentityField;
    static const Field ServiceStartField;
    static const Field ServiceDurationField;
    static const Field ServiceCostField;
    static const Field ServiceDescriptionField;
    static const Field ServiceObservationsField;

    ServiceFormModel();

    // Get model data
    EventDataModule::StructService getData();

private:
    // Text Validator
    std::shared_ptr<Wt::WValidator> createTextValidator(bool mandatory, int min, int max);

    // Event Start Time Validator
    std::shared_ptr<Wt::WValidator> createEventStartTimeValidator();

    // Event Duration Validator
    std::shared_ptr<Wt::WValidator> createEventDurationValidator(double min, double max);

    // Event Cost Validator
    std::shared_ptr<Wt::WValidator> createEventCostValidator(int min, int max);
};

// Service Form View
class ServiceFormView : public Wt::WTemplateFormView
{
public:
    ServiceFormView();

    std::shared_ptr<ServiceFormModel> model_;

private:
};
