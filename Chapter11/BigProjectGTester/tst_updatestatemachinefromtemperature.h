#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "HVACController.h"
#include <QString>
#include <QStringList>
#include <string>

using namespace testing;

TEST(BigProjectTests, sampleTest)
{
    EXPECT_EQ(1, 1);
    ASSERT_THAT(0, Eq(0));
}

struct smftData {
    QString     name;
    QStringList	states;
    float	min;
    float	max;
    float	current;
    QString	transition;

    std::string toStr() {
        QString v = QString("%1 - %2 | %3 | %4 | %5 | %6").arg(name).arg(states.join(":")).arg(min).arg(max).arg(current).arg(transition);
        return v.toStdString();
    }
};

class UpdateStateMachineFromTemperatureTests : public ::testing::TestWithParam<smftData> {

};

TEST_P(UpdateStateMachineFromTemperatureTests, coverageTest)
{
    auto p = GetParam();

    auto result = HVACController::updateSMfromTemperature(p.min, p.max, p.current, p.states);
    EXPECT_EQ(result.toStdString(), p.transition.toStdString()) << "Incorrect Transition for " << p.toStr();
}

smftData    fullCoverage[] = {
    {QString("Idle-Between")   , QStringList({"FanOff", "Idle"}) , 10.0f , 15.0f , 12.5f , QString("none")},
    {QString("Idle-High")      , QStringList({"FanOff", "Idle"}) , 10.0f , 15.0f , 20.0f , QString("TooHot")},
    {QString("Idle-Low")       , QStringList({"FanOff", "Idle"}) , 10.0f , 15.0f ,  5.0f , QString("TooCold")},

    {QString("Heating-Between"), QStringList({"FanOn" , "Heating"}) , 10.0f , 15.0f , 12.5f , QString("none")},
    {QString("Heating-High")   , QStringList({"FanOn" , "Heating"}) , 10.0f , 15.0f , 20.0f , QString("TargetReached")},
    {QString("Heating-Low")    , QStringList({"FanOn" , "Heating"}) , 10.0f , 15.0f ,  5.0f , QString("TooCold")},

    {QString("Cooling-Between"), QStringList({"FanOn" , "Cooling"}) , 10.0f , 15.0f , 12.5f , QString("none")},
    {QString("Cooling-High")   , QStringList({"FanOn" , "Cooling"}) , 10.0f , 15.0f , 20.0f , QString("TooHot")},
    {QString("Cooling-Low")    , QStringList({"FanOn" , "Cooling"}) , 10.0f , 15.0f ,  5.0f , QString("TargetReached")}
};

INSTANTIATE_TEST_CASE_P(FullFMFTCoverageData,
                        UpdateStateMachineFromTemperatureTests,
                        ValuesIn(fullCoverage)
);
