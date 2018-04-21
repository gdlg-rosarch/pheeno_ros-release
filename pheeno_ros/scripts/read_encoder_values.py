#!/usr/bin/env python

import rospy
import sys
from std_msgs.msg import Int16


def get_args():
    """ Get arguments from rosrun for individual deployment. """
    parser = argparse.ArgumentParser(
        description="Obstacle avoidance python script."
    )

    # Required arguments
    parser.add_argument("-n", "--number",
                        action="store",
                        required=False,
                        help="Add a pheeno number namespace.",
                        default="")

    # The rationale behind rospy.myargv()[1:] is provided here:
    # https://groups.google.com/a/rethinkrobotics.com/forum/#!topic/brr-users/ErXVWhRmtNA
    return parser.parse_args(rospy.myargv()[1:])


def print_callback(msg, encoder_location):
    """ Print encoder values in a easily legible fashion.

    Callback directed to from the ROS Subscribers. These are for printing
    encoder values for the pheeno.

    Parameters
    ----------
    msg : Int16
        Message recieved from topic '/encoder_*'. LL, LR, RL, and RR specify
        the H-bridge location (Left or Right) and the motor location (Left or
        Right). For exampe, '/encoder_LL' refers to the Left H-bridge and the
        Left motor, respectively.
    encoder_location : str
        Location of encoder on the Pheeno. Will only come as one of four
        options (LL, LR, RL, RR).

    """
    global encoder_values
    encoder_values[encoder_location] = msg.data
    print("LL: %s\t LR: %s\t RL: %s\t RR: %s\n" %
          (encoder_values["LL"], encoder_values["LR"],
           encoder_values["RL"], encoder_values["RR"]))


if __name__ == "__main__":
    try:
        # Get arguments from argument parser.
        input_args = get_args()
        if input_args.number is "":
            pheeno_number = ""

        else:
            pheeno_number = "/pheeno_" + str(input_args.number)

        # Initialize Node
        sys.stdout.write("Initializing 'read_encoder' node...")
        rospy.init_node("read_encoder")
        sys.stdout.write("done!\n")

        # Create Subscriber for encoder values.
        sub_encoder_LL = rospy.Subscriber(
            pheeno_number + "/encoder_LL", Int16, print_callback,
            callback_args="LL")
        sub_encoder_LR = rospy.Subscriber(
            pheeno_number + "/encoder_LR", Int16, print_callback,
            callback_args="LR")
        sub_encoder_RL = rospy.Subscriber(
            pheeno_number + "/encoder_RL", Int16, print_callback,
            callback_args="RL")
        sub_encoder_RR = rospy.Subscriber(
            pheeno_number + "/encoder_RR", Int16, print_callback,
            callback_args="RR")

        # Other variables
        global encoder_values
        encoder_values = {"LL": 0, "LR": 0, "RL": 0, "RR": 0}

        rospy.spin()

    except rospy.ROSInterruptException:
        pass

    finally:
        rospy.loginfo("Exiting 'read_encoder' node.")
