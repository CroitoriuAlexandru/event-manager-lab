#include "include/EventForms.h"
#include <Wt/WTime.h>

// Validators
#include <Wt/WIntValidator.h>    // for Cost
#include <Wt/WLengthValidator.h> // for Name
#include <Wt/WDateValidator.h>   // for Date
#include <Wt/WDoubleValidator.h> // for Duration
#include <Wt/WTimeValidator.h>   // for Start hour
#include <stdexcept>

// Widgets
#include <Wt/WDateEdit.h>      // for Date
#include <Wt/WDoubleSpinBox.h> // for Duration
#include <Wt/WTextArea.h>      // for Observations
#include <Wt/WTimeEdit.h>      // for Event Start
#include <Wt/WPushButton.h>    // for Submit btn
#include <Wt/WText.h>          // for Form Info
#include <Wt/WInPlaceEdit.h>   // for Title

const Wt::WFormModel::Field EventFormModel::ClientNameField = "client-name";
const Wt::WFormModel::Field EventFormModel::ClientPhoneNumberField = "client-phone";
const Wt::WFormModel::Field EventFormModel::EventDateField = "event-date";
const Wt::WFormModel::Field EventFormModel::EventStartField = "event-start";
const Wt::WFormModel::Field EventFormModel::EventDurationField = "event-duration";
const Wt::WFormModel::Field EventFormModel::EventLocationField = "event-location";
const Wt::WFormModel::Field EventFormModel::EventObservationsField = "event-observations";

EventFormModel::EventFormModel()
    : WFormModel()
{

    addField(ClientNameField);
    addField(ClientPhoneNumberField);
    addField(EventDateField);
    addField(EventLocationField);
    addField(EventStartField);
    addField(EventDurationField);
    addField(EventObservationsField);

    setValidator(ClientNameField, createTextValidator(true, 3, 30));
    setValidator(ClientPhoneNumberField, createTextValidator(true, 12, 12));
    setValidator(EventDateField, createDateValidator());
    setValidator(EventStartField, createEventStartTimeValidator());
    setValidator(EventDurationField, createEventDurationValidator(1.0, 10.0));
    setValidator(EventLocationField, createTextValidator(true, 1, 50));
    setValidator(EventObservationsField, createTextValidator(false, 1, 300));

    // Initialize the fields with a default value
    // setValue(ClientPhoneNumberField, "07");
    setValue(ClientNameField, Wt::WString("Ioana Piros"));
    setValue(ClientPhoneNumberField, Wt::WString("0771133255"));
    setValue(EventDateField, Wt::WDate::currentServerDate().addDays(0));
    setValue(EventStartField, "12:00 PM");
    setValue(EventDurationField, "1");
    setValue(EventLocationField, "https://goo.gl/maps/GkqcccuMKcP3JaQG6");
    setValue(EventObservationsField, "Este o clienta veche, foarte buna");

    // "h:mm AP"
}

EventDataModule::StructRegisterEvent EventFormModel::getData()
{
    EventDataModule::StructRegisterEvent eventData;

    eventData.client.name = valueText(ClientNameField).toUTF8();
    eventData.client.phone = valueText(ClientPhoneNumberField).toUTF8();
    eventData.date = valueText(EventDateField).toUTF8();
    eventData.startHour = valueText(EventStartField).toUTF8();
    eventData.duration = ::atof(valueText(EventDurationField).toUTF8().c_str());
    eventData.location = valueText(EventLocationField).toUTF8();
    eventData.observations = valueText(EventObservationsField).toUTF8();

    return eventData;
}

// Name Validator
std::shared_ptr<Wt::WValidator> EventFormModel::createTextValidator(bool mandatory, int min, int max)
{
    auto v = std::make_shared<Wt::WLengthValidator>();
    v->setMandatory(mandatory);
    v->setMinimumLength(min);
    v->setMaximumLength(max);
    return v;
}

// Date Validator
std::shared_ptr<Wt::WValidator> EventFormModel::createDateValidator()
{
    auto v = std::make_shared<Wt::WDateValidator>();
    v->setMandatory(true);
    v->setBottom(Wt::WDate::currentDate());
    v->setTop(Wt::WDate::currentDate().addMonths(5));
    v->setFormat("dd/MM/yyyy");
    v->setMandatory(true);
    return v;
}

// Event Start Time Validator
std::shared_ptr<Wt::WValidator> EventFormModel::createEventStartTimeValidator()
{
    auto v = std::make_shared<Wt::WTimeValidator>("HH:mm AP");
    v->setMandatory(true);
    return v;
}

// Event Duration Validator
std::shared_ptr<Wt::WValidator> EventFormModel::createEventDurationValidator(double min, double max)
{
    auto v = std::make_shared<Wt::WDoubleValidator>();
    v->setMandatory(true);
    v->setTop(max);
    v->setBottom(min);
    return v;
}

EventFormView::EventFormView()
{
    model_ = std::make_shared<EventFormModel>();

    setTemplateText(tr("event-registration-form"));
    addFunction("id", &WTemplate::Functions::id);

    /*
     * Client Name Widget
     */

    auto clientNameWidget = std::make_unique<Wt::WLineEdit>();
    clientNameWidget->setPlaceholderText("this is needed for input desighn animation");
    setFormWidget(model_->ClientNameField, std::move(clientNameWidget));

    /*
     * Client Phone Widget
     */
    auto clientPhoneNumberWidget = std::make_unique<Wt::WLineEdit>();
    clientPhoneNumberWidget->setInputMask("9999.999.999");
    clientPhoneNumberWidget->setPlaceholderText("this is needed for input desighn animation");
    setFormWidget(model_->ClientPhoneNumberField, std::move(clientPhoneNumberWidget));

    /*
     * Event Date Widget
     */
    auto eventDateWidget = std::make_unique<Wt::WDateEdit>();

    eventDateWidget->setReadOnly(true);
    eventDateWidget->setPlaceholderText("this is needed for input desighn animation");
    auto eventDate_ = eventDateWidget.get();
    setFormWidget(
        model_->EventDateField, std::move(eventDateWidget),
        [=] { // updateViewValue()
            Wt::WDate date = Wt::cpp17::any_cast<Wt::WDate>(model_->value(model_->EventDateField));
            eventDate_->setDate(date);
        },

        [=] { // updateModelValue()
            Wt::WDate date = eventDate_->date();
            model_->setValue(model_->EventDateField, date);
        });

    /*
     * Event Starting Time Widget
     */
    auto eventStartWidget = std::make_unique<Wt::WTimeEdit>();
    eventStartWidget->setMinuteStep(15);
    eventStartWidget->setTime(Wt::WTime(12, 0, 0));
    eventStartWidget->setFormat("HH:mm AP");
    eventStartWidget->setReadOnly(true);
    setFormWidget(model_->EventStartField, std::move(eventStartWidget));

    /*
     * Event Duration Widget
     */
    auto eventDurationWidget = std::make_unique<Wt::WDoubleSpinBox>();
    eventDurationWidget->setDecimals(1);
    eventDurationWidget->setSingleStep(0.5);
    setFormWidget(model_->EventDurationField, std::move(eventDurationWidget));

    /*
     * Event Location Widget
     */
    auto eventLocation = std::make_unique<Wt::WLineEdit>();
    eventLocation->setPlaceholderText("this is needed for input desighn animation");
    setFormWidget(model_->EventLocationField, std::move(eventLocation));

    /*
     *  Observations Textarea Widget
     */
    auto observationsWidget = std::make_unique<Wt::WTextArea>();
    setFormWidget(model_->EventObservationsField, std::move(observationsWidget));
    bindString("submit-info", Wt::WString(""));
    updateView(model_.get());
}

void EventFormView::process()
{
    updateModel(model_.get());

    if (model_->validate())
    {
        model_->getData();
        updateView(model_.get());
    }
    else
    {
        bindString("submit-info", Wt::WString("campuri obligatorii necompletate"));
        updateView(model_.get());
    }
}

const Wt::WFormModel::Field ServiceFormModel::ServiceProviderIdentityField = "provider-identity";
const Wt::WFormModel::Field ServiceFormModel::ServiceStartField = "service-start";
const Wt::WFormModel::Field ServiceFormModel::ServiceDurationField = "service-duration";
const Wt::WFormModel::Field ServiceFormModel::ServiceCostField = "service-cost";
const Wt::WFormModel::Field ServiceFormModel::ServiceDescriptionField = "service-description";
const Wt::WFormModel::Field ServiceFormModel::ServiceObservationsField = "service-observations";

ServiceFormModel::ServiceFormModel()
    : WFormModel()
{
    addField(ServiceProviderIdentityField);
    addField(ServiceStartField);
    addField(ServiceDurationField);
    addField(ServiceCostField);
    addField(ServiceDescriptionField);
    addField(ServiceObservationsField);

    setValidator(ServiceProviderIdentityField, createTextValidator(false, 0, 30));
    setValidator(ServiceStartField, createEventStartTimeValidator());
    setValidator(ServiceDurationField, createEventDurationValidator(1.0, 10.0));
    setValidator(ServiceCostField, createEventCostValidator(0, 100000));
    setValidator(ServiceDescriptionField, createTextValidator(false, 0, 50));
    setValidator(ServiceObservationsField, createTextValidator(false, 0, 300));

    // Initialize the fields with a default value
    setValue(ServiceProviderIdentityField, "Andreea");
    setValue(ServiceStartField, "12:00 PM");
    setValue(ServiceDurationField, "1");
    setValue(ServiceCostField, 200);
    setValue(ServiceDescriptionField, "service description");
    setValue(ServiceDurationField, 1);

    // "h:mm AP"3'
}

// Do something with the data from the model
EventDataModule::StructService ServiceFormModel::getData()
{
    EventDataModule::StructService serviceData;

    serviceData.providerIdentity = valueText(ServiceProviderIdentityField).toUTF8();
    serviceData.startHour = valueText(ServiceStartField).toUTF8();
    serviceData.duration = std::atof(valueText(ServiceDurationField).toUTF8().c_str());
    serviceData.cost = std::stoi(valueText(ServiceCostField).toUTF8());
    serviceData.description = valueText(ServiceDescriptionField).toUTF8();
    serviceData.observations = valueText(ServiceObservationsField).toUTF8();

    return serviceData;
}
// Text Validator
std::shared_ptr<Wt::WValidator> ServiceFormModel::createTextValidator(bool mandatory, int min, int max)
{
    auto v = std::make_shared<Wt::WLengthValidator>();
    v->setMandatory(mandatory);
    v->setMinimumLength(min);
    v->setMaximumLength(max);
    return v;
}

// Event Start Time Validator
std::shared_ptr<Wt::WValidator> ServiceFormModel::createEventStartTimeValidator()
{
    auto v = std::make_shared<Wt::WTimeValidator>("HH:mm AP");
    v->setMandatory(true);
    return v;
}

// Event Duration Validator
std::shared_ptr<Wt::WValidator> ServiceFormModel::createEventDurationValidator(double min, double max)
{
    auto v = std::make_shared<Wt::WDoubleValidator>();
    v->setMandatory(true);
    v->setTop(max);
    v->setBottom(min);
    return v;
}
// Event Cost Validator
std::shared_ptr<Wt::WValidator> ServiceFormModel::createEventCostValidator(int min, int max)
{
    auto v = std::make_shared<Wt::WIntValidator>();
    v->setTop(max);
    v->setBottom(min);
    return v;
}

ServiceFormView::ServiceFormView()
    : WTemplateFormView(),
      model_(std::make_shared<ServiceFormModel>())
{
    setTemplateText(tr("service-form"));
    addFunction("id", &Wt::WTemplate::Functions::id);
    /*
     * Service Provider Identity Widget
     */

    auto serviceProviderIdentity = std::make_unique<Wt::WLineEdit>();
    serviceProviderIdentity->setPlaceholderText("this is needed for input desighn animation");
    setFormWidget(model_->ServiceProviderIdentityField, std::move(serviceProviderIdentity));

    /*
     * Service Starting Time Widget
     */
    auto serviceStartWidget = std::make_unique<Wt::WTimeEdit>();
    serviceStartWidget->setMinuteStep(15);
    serviceStartWidget->setTime(Wt::WTime(12, 0, 0));
    serviceStartWidget->setFormat("HH:mm AP");
    serviceStartWidget->setReadOnly(true);
    setFormWidget(model_->ServiceStartField, std::move(serviceStartWidget));

    /*
     * Service Duration Widget
     */
    auto serviceDurationWidget = std::make_unique<Wt::WDoubleSpinBox>();
    serviceDurationWidget->setDecimals(1);
    serviceDurationWidget->setSingleStep(0.5);
    setFormWidget(model_->ServiceDurationField, std::move(serviceDurationWidget));

    /*
     * Service Cost Widget
     */

    auto serviceCostWidget = std::make_unique<Wt::WSpinBox>();
    serviceCostWidget->setSingleStep(25);
    serviceCostWidget->setMaximum(100000);
    setFormWidget(model_->ServiceCostField, std::move(serviceCostWidget));

    /*
     *  Description Textarea Widget
     */
    auto serviceDescriptionWidget = std::make_unique<Wt::WTextArea>();
    setFormWidget(model_->ServiceDescriptionField, std::move(serviceDescriptionWidget));

    /*
     *  Observations Textarea Widget
     */
    auto observationsWidget = std::make_unique<Wt::WTextArea>();
    setFormWidget(model_->ServiceObservationsField, std::move(observationsWidget));

    bindString("submit-info", Wt::WString(""));

    updateView(model_.get());
}
