/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2008, 2013, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Eitan Marder-Eppstein
 *         David V. Lu!!
 *********************************************************************/
#ifndef COSTMAP_2D_OBSTACLE_LAYER_H_
#define COSTMAP_2D_OBSTACLE_LAYER_H_

#include "costmap_layer.h"
#include "LayeredCostmap.h"
#include "observation_buffer.h"

#include "footprint.h"
#include "observation.h"
#include "sensor_msgs.h"
#include "nav_msgs.h"

namespace costmap_core
{

    class ObstacleLayer : public CostmapLayer
    {
    public:
        ObstacleLayer()
        {
            // this is the unsigned char* member of parent class Costmap2D.
            costmap_ = NULL;
        }

        virtual ~ObstacleLayer();

        virtual void onInitialize();

        virtual void updateBounds(double robot_x, double robot_y,
                                  double robot_yaw,
                                  double *min_x, double *min_y,
                                  double *max_x, double *max_y);

        virtual void updateCosts(costmap_core::Costmap2D &master_grid,
                                 int min_i, int min_j, int max_i,
                                 int max_j);

        virtual void activate();

        virtual void deactivate();

        virtual void reset();

        /**
         * @brief  A callback to handle buffering LaserScan messages
         * @param message The message returned from a message notifier
         * @param buffer A pointer to the observation buffer to update
         */
        void laserScanCallback(const base_info::LaserScan &message,
                               const std::shared_ptr<costmap_core::ObservationBuffer>
                               &buffer);

        /**
         * @brief A callback to handle buffering LaserScan messages which need filtering
         * to turn Inf values into range_max.
         * @param message The message returned from a message notifier
         * @param buffer A pointer to the observation buffer to update
         */
        void laserScanValidInfCallback(const base_info::LaserScan &message,
                                       const std::shared_ptr<ObservationBuffer> &buffer);

        void clearStaticObservations(bool marking, bool clearing);

    protected:
        /**
         * @brief  Get the observations used to mark space
         * @param marking_observations
         * A reference to a vector that will be populated with the observations
         * @return True if all the observation buffers are current, false otherwise
         */
        bool getMarkingObservations(std::vector<costmap_core::Observation>
                                    &marking_observations) const;

        /**
         * @brief  Get the observations used to clear space
         * @param clearing_observations A reference to a vector that
         * will be populated with the observations
         * @return True if all the observation buffers are current, false otherwise
         */
        bool getClearingObservations(
                std::vector<costmap_core::Observation> &clearing_observations) const;

        /**
         * @brief  Clear freespace based on one observation
         * @param clearing_observation The observation used to raytrace
         * @param min_x
         * @param min_y
         * @param max_x
         * @param max_y
         */
        virtual void raytraceFreespace(const costmap_core::Observation &clearing_observation,
                                       double *min_x,
                                       double *min_y,
                                       double *max_x, double *max_y);

        void updateRaytraceBounds(double ox, double oy, double wx, double wy,
                                  double range,
                                  double *min_x, double *min_y,
                                  double *max_x, double *max_y);

        std::vector<base_info::bd_Point> transformed_footprint_;
        bool footprint_clearing_enabled_;

        void updateFootprint(double robot_x, double robot_y, double robot_yaw,
                             double *min_x,
                             double *min_y,
                             double *max_x, double *max_y);

        int global_frame_;  ///< @brief The global frame for the costmap
        double max_obstacle_height_;  ///< @brief Max Obstacle Height

        ///< @brief Used to store observations from various sensors
        std::vector<std::shared_ptr<costmap_core::ObservationBuffer> > observation_buffers_;
        ///< @brief Used to store observation buffers used for marking obstacles
        std::vector<std::shared_ptr<costmap_core::ObservationBuffer> > marking_buffers_;
        ///< @brief Used to store observation buffers used for clearing obstacles
        std::vector<std::shared_ptr<costmap_core::ObservationBuffer> > clearing_buffers_;

        // Used only for testing purposes
        std::vector<costmap_core::Observation> static_clearing_observations_,
                static_marking_observations_;

        bool rolling_window_;

        int combination_method_;
    };

}  // namespace costmap_2d

#endif  // COSTMAP_2D_OBSTACLE_LAYER_H_
