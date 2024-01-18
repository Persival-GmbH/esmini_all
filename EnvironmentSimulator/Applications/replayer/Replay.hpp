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

#include <string>
#include <fstream>
#include "CommonMini.hpp"
#include "ScenarioGateway.hpp"
#include "DatLogger.hpp"

namespace scenarioengine
{
    typedef struct
    {
        ObjectStateStructDat state;
        double               odometer;
    } ReplayEntry;

    // new replayer
    // cache for reading states
    typedef struct
    {
        double time_;
        char*  pkg;
    } ObjectStateWithPkg;

    typedef struct
    {
        int                             id;
        bool                            active;
        std::vector<ObjectStateWithPkg> pkgs;
    } ObjectStateWithObjId;

    struct ScenarioState
    {
        double                            sim_time;
        double                            odometer = 0.0;
        std::vector<ObjectStateWithObjId> obj_states;
    };

    typedef struct
    {
        double                  sim_time;
        int                     obj_id;
    } ScenarioEntities;

    typedef struct
    {
        double                  restart_time_;
        double                  next_time_ = LARGE_NUMBER;
        unsigned int            restart_index_;
        unsigned int            next_index_;
    } RestartTimes;


    class Replay
    {
    public:
        datLogger::CommonPkg     header_;
        std::vector<ReplayEntry> data_;

        // Replay(std::string filename, bool clean);
        // Replay(const std::string directory, const std::string scenario, bool clean);
        // Replay(const std::string directory, const std::string scenario, std::string create_datfile);
        Replay(const std::string directory, const std::string ScenarioEntities, std::string create_datfile);

        // new replayer
        Replay(std::string filename);

        ~Replay();

        /**
                Go to specific time
                @param time timestamp (0 = beginning, -1 end)
                @param stop_at_next_frame If true move max to next/previous time frame
        */
        void                  GetReplaysFromDirectory(const std::string dir, const std::string sce);
        size_t                GetNumberOfScenarios();


        void SetRepeat(bool repeat)
        {
            repeat_ = repeat;
        }
        void CleanEntries(std::vector<ReplayEntry>& entries);
        void BuildData(std::vector<std::pair<std::string, std::vector<ReplayEntry>>>& scenarios);
        int CreateMergedDatfile(const std::string filename);

        // new merge dat methods
        std::vector<std::pair<std::string, std::vector<datLogger::CommonPkg>>> scenarioData;
        void BuildData();
        double GetLastTime();
        std::vector<int> objectIds;
        void AdjustObjectId( std::vector<std::vector<int>>& objectIds);

        // vector and method for record and read pkg
        std::vector<datLogger::CommonPkg>   pkgs_;
        ScenarioState                       scenarioState;
        int                                 RecordPkgs(const std::string& fileName);  // check package can be recorded or not
        void                                GetScenarioEntities(); // get all the entities in complete scenario
        std::vector<int>                    GetNumberOfObjectsAtTime();       // till next time forward
        int                                 GetPkgCntBtwObj(size_t idx);              // till next time forward
        datLogger::PackageId                ReadPkgHdr(char* package);
        double                              GetTimeFromCnt(int count);            // give time for the time
        void GetRestartTimes();
        std::vector<RestartTimes> restartTimes;


        // method for cache
        void    InitiateStates();
        void    UpdateCache();
        void    AddObjState(size_t objId, double t);  // add the object state for given object id from the current object state
        void    deleteObjState(int objId);
        /**
         Move to specified timestamp
         @param time_frame Timestamp
         @param goToEnd In case to go to end, Will be faster.
         @param stopAtEachFrame Always fill cache for each time frame
        */
        int     MoveToTime(double time_frame, bool goToEnd = false, bool stopAtEachFrame = false);
        bool    MoveToNextFrame(double t);
        bool    MoveToPreviousFrame(double t);
        void    MoveToDeltaTime(double dt);
        void    MoveToStart();
        void    MoveToEnd();
        bool    IsObjAvailableInCache(int Id);  // check in cache
        bool    IsObjAvailableActive(int id);
        int     UpdateObjStatus( int id, bool status);
        std::vector<ScenarioEntities> entities;
        double GetTimeFromEntities(int id);
        void CheckObjAvailabilityForward();
        void CheckObjAvailabilityBackward();

        // method to handle time and index
        void                  SetStartTime(double time);
        void                  SetStopTime(double time);
        double                GetStartTime()
        {
            return startTime_;
        }
        double GetStopTime()
        {
            return stopTime_;
        }
        double GetTime()
        {
            return scenarioState.sim_time;
        }
        int GetIndex()
        {
            return static_cast<int>(index_);
        }

        void SetTime(double t)
        {
            time_ = t;
        }
        void SetIndex( int index)
        {
            index_ = static_cast<unsigned int>(index);
        }



        // method to read data from cache
        int             GetModelID(int obj_id);
        int             GetCtrlType(int obj_id);
        int             GetBB(int obj_id, OSCBoundingBox& bb);
        int             GetScaleMode(int obj_id);
        int             GetVisibility(int obj_id);
        datLogger::Pos  GetPos(int obj_id);
        double          GetX(int obj_id);
        double          GetY(int obj_id);
        double          GetZ(int obj_id);
        double          GetH(int obj_id);
        double          GetR(int obj_id);
        double          GetP(int obj_id);
        int             GetRoadId(int obj_id);
        int             GetLaneId(int obj_id);
        double          GetPosOffset(int obj_id);
        float           GetPosT(int obj_id);
        float           GetPosS(int obj_id);
        ObjectPositionStructDat GetComPletePos(int obj_id);
        double          GetWheelAngle(int obj_id);
        double          GetWheelRot(int obj_id);
        double          GetSpeed(int obj_id);
        int             GetName(int obj_id, std::string& name);

        void UpdateOdaMeter(double value)
        {
            scenarioState.odometer = value;
        }
        void SetStopEntries();
        double                   deltaTime_ = LARGE_NUMBER;

        void SetShowRestart( bool showRestart)
        {
            show_restart_  = showRestart;
        }

    private:
        std::ifstream            file_;
        std::vector<std::string> scenarios_;
        double                   time_;
        double                   startTime_;
        double                   stopTime_;
        unsigned int             startIndex_;
        unsigned int             stopIndex_;
        unsigned int             index_;
        bool                     repeat_;
        bool                     clean_;
        std::string              create_datfile_;
        bool                     show_restart_;
        bool                     IsRestart = false;

        double                   previousTime_ = std::nan("");

        datLogger::DatLogger *datLogger = nullptr;
        int FindIndexAtTimestamp(double timestamp, int startSearchIndex = 0);
    };

}  // namespace scenarioengine