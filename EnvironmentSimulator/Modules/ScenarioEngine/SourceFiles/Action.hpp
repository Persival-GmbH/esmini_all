/*
 * esmini - Environment Simulator Minimalistic
 * https://github.com/esmini/esmini
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) partners of Simulation Scenarios
 * https://sites.google.com/view/simulationscenarios
 */

#pragma once

#include "StoryboardElement.hpp"

namespace scenarioengine
{
    class OSCAction : public StoryBoardElement
    {
    public:
        typedef enum
        {
            GLOBAL,
            USER_DEFINED,
            PRIVATE,
        } BaseType;

        BaseType base_type_;

        OSCAction(BaseType type, StoryBoardElement* parent) : StoryBoardElement(StoryBoardElement::ElementType::ACTION, parent), base_type_(type)
        {
            id_ = OSCAction::CreateUniqeActionId();
        }

        virtual ~OSCAction()
        {
        }

        std::string         BaseType2Str();
        virtual std::string Type2Str()
        {
            return BaseType2Str();
        }

        unsigned int GetNumChildren()
        {
            return 0;
        }

        StoryBoardElement* GetChild(unsigned int index)
        {
            (void)index;
            return nullptr;
        }

        std::vector<StoryBoardElement*>* GetChildren() override
        {
            return &dummy_child_list_;
        }

        unsigned int GetId() const
        {
            return id_;
        }

    private:
        // add dummy child list to avoid nullptr checks - don't add elments to this list
        std::vector<StoryBoardElement*> dummy_child_list_;
        unsigned int                    id_;  // unique ID for each action
        static unsigned int             n_actions_;
        static unsigned int             CreateUniqeActionId()
        {
            return n_actions_++;
        }
    };

    class OSCUserDefinedAction : public OSCAction
    {
    public:
        OSCUserDefinedAction(StoryBoardElement* parent) : OSCAction(OSCAction::BaseType::USER_DEFINED, parent)
        {
        }

        OSCUserDefinedAction(const OSCUserDefinedAction& action) : OSCAction(OSCAction::BaseType::USER_DEFINED, action.parent_)
        {
            SetName(action.GetName());
            type_    = action.type_;
            content_ = action.content_;
        }

        ~OSCUserDefinedAction()
        {
        }

        OSCUserDefinedAction* Copy()
        {
            OSCUserDefinedAction* new_action = new OSCUserDefinedAction(*this);
            return new_action;
        }

        std::string Type2Str()
        {
            return "UserDefinedAction";
        };

        void Start(double simTime) override
        {
            LOG("Starting %s type: %s content: %s", Type2Str().c_str(), type_.c_str(), content_.c_str());
            OSCAction::Start(simTime);
        }

        void Step(double, double) override
        {
        }

        std::string type_;
        std::string content_;
    };

}  // namespace scenarioengine