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

/*
 * This controller simulates a bad or dizzy driver by manipulating 
 * the speed and lateral offset in a random way.
 * The purpose is purely to demonstrate how to implement a controller.
 */

//#include "playerbase.hpp"
#include "ControllerDriverModel.hpp"
#include "CommonMini.hpp"
#include "Entities.hpp"
#include "ScenarioGateway.hpp"

using namespace scenarioengine;

Controller* scenarioengine::InstantiateControllerDriverModel(std::string name, Entities* entities, ScenarioGateway* gateway,
	Parameters* parameters, OSCProperties* properties)
{
	return new ControllerDriverModel(name, entities, gateway, parameters, properties);
}

ControllerDriverModel::ControllerDriverModel(std::string name, Entities* entities, ScenarioGateway* gateway,
	Parameters* parameters, OSCProperties* properties) :
	Controller(name, properties, entities, gateway, parameters)
{
}

void ControllerDriverModel::Init()
{
	Controller::Init();
}

void ControllerDriverModel::PostFrame()
{
	Controller::PostFrame();
}

void ControllerDriverModel::Step(double timeStep)
{
#if 0
	// Set steering target point at a distance ahead proportional to the speed
	double probe_target_distance = MAX(7, 0.5 * object_->speed_);

	// Find out a steering target along ghost vehicle trail
	double s_out;
	int index_out;
	ObjectTrailState state;

	// Locate a point at given distance from own vehicle along the ghost trajectory
	if (ghost_ && ghost_->trail_.FindPointAhead(
		object_->trail_follow_index_, object_->trail_follow_s_, probe_target_distance, state, index_out, s_out) != 0)
	{
		state.x_ = (float)object_->pos_.GetX();
		state.y_ = (float)object_->pos_.GetY();
		state.z_ = (float)object_->pos_.GetZ();
		state.speed_ = 0;
	}

	// Update object sensor position for visualization
	object_->sensor_pos_[0] = state.x_;
	object_->sensor_pos_[1] = state.y_;
	object_->sensor_pos_[2] = state.z_;

	double diffGlobal[2] = { state.x_ - object_->pos_.GetX(), state.y_ - object_->pos_.GetY() };
	double len = sqrt(diffGlobal[0] * diffGlobal[0] + diffGlobal[1] * diffGlobal[1]);
	if (len > SMALL_NUMBER)
	{
		diffGlobal[0] /= len;
		diffGlobal[1] /= len;
	}
	else
	{
		diffGlobal[0] = 0;
		diffGlobal[1] = 0;
	}

	// Find heading to the point
	double egoDirGlobal[2];
	RotateVec2D(1.0, 0.0, object_->pos_.GetH(), egoDirGlobal[0], egoDirGlobal[1]);

	double diffH = asin(GetCrossProduct2D(egoDirGlobal[0], egoDirGlobal[1], diffGlobal[0], diffGlobal[1]));
#endif
	// Update driver model target values
	vehicle_.DrivingControlTarget(timeStep, 0, 10);

	// Register updated vehicle position 
	object_->pos_.XYZH2TrackPos(vehicle_.posX_, vehicle_.posY_, vehicle_.posZ_, vehicle_.heading_);

	// Report updated state to scenario gateway
	gateway_->reportObject(object_->id_, object_->name_, static_cast<int>(Object::Type::VEHICLE), static_cast<int>(Vehicle::Category::CAR),
		0, object_->GetControllerType(), object_->boundingbox_, 0,
		vehicle_.speed_, vehicle_.wheelAngle_, vehicle_.wheelRotation_,
		vehicle_.posX_, vehicle_.posY_, vehicle_.posZ_,
		vehicle_.heading_, vehicle_.pitch_, 0);

	Controller::Step(timeStep);
}

void ControllerDriverModel::Activate(int domainMask)
{

	Controller::Activate(domainMask);
}

void ControllerDriverModel::ReportKeyEvent(int key, bool down)
{

}