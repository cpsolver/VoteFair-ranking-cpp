//  measure_rcipe_stv_satisfaction_rate.cpp
//
//  This software utility generates random ballots
//  for the rcipe_stv.cpp code and then measures
//  the fairness of the results.
//
//  One of the measurements is how often the
//  rcipe_stv.cpp software yields results that are
//  different from the results of calcuating
//  VoteFair representation winners -- which use
//  an advanced counting method that ensures
//  fully proportional representation for
//  filling two equivalent seats, and does not
//  reward tactical voting.
//
//  Eventually this software will calculate
//  "satisfaction" scores that measure how well
//  the elected candidates match the ideal
//  desires of the voters.  The expected name of
//  those measurements is Proportional Pairwise
//  Support Percent (PPSP)
//
//
// -----------------------------------------------
//
//  Tip:
//
//  Jump ahead to "settings_here" to find the
//  current settings for the number of available
//  seats, the number of ballots, ballot markings,
//  etc.
//
//
// -----------------------------------------------
//
//  COPYRIGHT & LICENSE
//
//  (c) Copyright 2022 by Richard Fobes at
//  www.VoteFair.org.  You can redistribute and/or
//  modify this VoteFairRanking software under the
//  MIT software license terms that appear in the
//  "license" file.
//
//  Conversion of this code into another
//  programming language is also covered by the
//  above license terms.
//
//
// -----------------------------------------------
//
//  VERSION
//
//  Version 0.10 - Spring 2022 created and refined
//  this software.
//
//  Version 0.50 - June 2022 getting successful
//  test results.
//
//
// -----------------------------------------------
//
//  USAGE
//
//  The following sample code compiles and then
//  executes this software under a typical Windows
//  environment with the g++ compiler and the
//  mingw32 library already installed.
//
//      path=C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin\
//
//      g++ measure_rcipe_stv_satisfaction_rate.cpp -o measure_rcipe_stv_satisfaction_rate
//
//      .\measure_rcipe_stv_satisfaction_rate > output_measure_rcipe_stv_satisfaction_rate.txt
//
//
// -----------------------------------------------
//
//  LICENSE
//
//  This measure_rcipe_stv_satisfaction_rate software
//  is licensed under the MIT License terms.
//
//
// -----------------------------------------------


// -----------------------------------------------
//  Begin code.
// -----------------------------------------------


// -----------------------------------------------
//  Specify libraries needed.

#include <cstring>
#include <string>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>


// -----------------------------------------------
//  Define yes and no constants.

const int global_no = 0 ;
const int global_yes = 1 ;


// -----------------------------------------------
//  Declare global variables.

int global_case_id ;
int global_minimum_case_id ;
int global_case_count_limit ;
int global_number_of_case_types_to_test ;
int global_case_type ;
int global_case_type_votefair_popularity ;
int global_case_type_votefair_representation ;
int global_case_type_plurality ;
int global_case_type_random ;
int global_total_ballot_count ;
int global_number_of_tests ;
int global_count_ballot_generation_done ;
int global_full_candidate_count ;
int global_count_of_cases_ignored ;
int global_maximum_ballot_group ;
int global_maximum_ranking_level ;
int global_counter_for_ballot_pattern_shared_rankings ;
int global_test_count ;
int global_count_of_tests_that_match ;
int global_candidate_winner ;
int global_plurality_winner ;
int global_majority_winner ;
int global_candidate_omitted ;
int global_yes_or_no_show_details_in_log_file ;
int global_count_of_seats_filled ;
int global_ballot_count_times_candidate_count_minus_one ;
int global_pointer_to_list_voteinfo_output_begin ;
int global_pointer_to_list_voteinfo_output_ballot ;
int global_pointer_to_list_voteinfo_output_end ;


// -----------------------------------------------
//  Declare all the arrays here so that they do
//  not require the overhead (time delays)
//  associated with local arrays.  Also specify
//  the maximum list lengths.

const int global_maximum_number_of_case_types = 40 ;
int global_list_of_case_types_to_test[ 41 ] ;
int global_number_of_seats_to_fill_for_case_type[ 41 ] ;
int global_yes_or_no_eliminate_pairwise_losing_candidate_for_case_type[ 41 ] ;
int global_yes_or_no_switch_to_droop_quota_for_case_type[ 41 ] ;
int global_yes_or_no_request_ignore_shared_rankings_for_case_type[ 41 ] ;
int global_count_of_successful_cases_for_case_type[ 41 ] ;

const int global_maximum_number_of_seats_to_fill = 10 ;
int global_candidate_who_won_seat_number[ 11 ] ;
int global_candidate_who_won_votefair_seat_number[ 11 ] ;

const int global_maximum_number_of_candidates = 20 ;
int global_count_first_choice_usage_of_candidate[ 21 ] ;

const int global_maximum_number_of_ranking_levels = 60 ;
int global_candidate_number_at_picking_position[ 61 ] ;

const int global_maximum_number_of_ballot_groups = 200 ;
int global_repeat_count_for_ballot_group[ 201 ] ;
int global_list_of_supporting_ballot_groups[ 201 ] ;
float global_decimal_remaining_influence_for_ballot_group[ 201 ] ;
float global_decimal_reduced_influence_for_ballot_group[ 201 ] ;

const int global_maximum_number_of_voteinfo_output_codes = 1000 ;
int global_list_voteinfo_output_begin[ 1001 ] ;
int global_list_voteinfo_output_ballot[ 1001 ] ;
int global_list_voteinfo_output_end[ 1001 ] ;

const int global_maximum_number_of_result_codes = 2000 ;
int global_result_code_at_position[ 2001 ] ;

int global_result_code_at_prior_position[ 11 ] ;

const int global_maximum_number_of_result_categories = 20 ;
int global_results_category_number_for_case_type[ 21 ] ;

int global_usage_count_for_candidate_and_sequence_position[ 21 ][ 61 ] ;
int global_sequence_position_for_candidate_and_ballot_group[ 21 ][ 201 ] ;
int global_ranking_for_candidate_and_ballot_group[ 21 ][ 201 ] ;


// -----------------------------------------------
//  Specify codes that identify the meaning of the
//  next number in the coded output list.

const int global_voteinfo_code_for_start_of_all_cases = -1 ;
const int global_voteinfo_code_for_end_of_all_cases = -2 ;
const int global_voteinfo_code_for_case_number = -3 ;
const int global_voteinfo_code_for_question_number = -4 ;

//  in other software the following constant is named global_voteinfo_code_for_number_of_choices
const int global_voteinfo_code_for_number_of_candidates = -6 ;

const int global_voteinfo_code_for_start_of_all_vote_info = -7 ;
const int global_voteinfo_code_for_end_of_all_vote_info = -8 ;
const int global_voteinfo_code_for_end_of_ballot = -10 ;
const int global_voteinfo_code_for_ballot_count = -11 ;
const int global_voteinfo_code_for_preference_level = -12 ;
const int global_voteinfo_code_for_choice = -13 ;
const int global_voteinfo_code_for_tie = -14 ;
const int global_voteinfo_code_for_start_of_votefair_popularity_ranking_sequence_results = -15 ;
const int global_voteinfo_code_for_end_of_votefair_popularity_ranking_sequence_results = -16 ;
const int global_voteinfo_code_for_start_of_votefair_popularity_ranking_levels_results = -17 ;
const int global_voteinfo_code_for_end_of_votefair_popularity_ranking_levels_results = -18 ;
const int global_voteinfo_code_for_start_of_votefair_representation_ranking_sequence_results = -19 ;
const int global_voteinfo_code_for_end_of_votefair_representation_ranking_sequence_results = -20 ;
const int global_voteinfo_code_for_start_of_votefair_representation_ranking_levels_results = -21 ;
const int global_voteinfo_code_for_end_of_votefair_representation_ranking_levels_results = -22 ;
const int global_voteinfo_code_for_next_ranking_level = -28 ;
const int global_voteinfo_code_for_request_votefair_representation_rank = -41 ;
const int global_voteinfo_code_for_request_no_votefair_representation_rank = -42 ;
const int global_voteinfo_code_for_request_instant_runoff_voting = -50 ;
const int global_voteinfo_code_for_request_rcipe_voting = -52 ;
const int global_voteinfo_code_for_winner_instant_runoff_voting = -53 ;
const int global_voteinfo_code_for_winner_rcipe_voting = -55 ;
const int global_voteinfo_code_for_flag_as_interesting = -62 ;
const int global_voteinfo_code_for_request_logging_off = -65 ;
const int global_voteinfo_code_for_winner_pairwise_support_count = -66 ;
const int global_voteinfo_code_for_number_of_equivalent_seats = -67 ;
const int global_voteinfo_code_for_request_quota_droop_not_hare = -68 ;
const int global_voteinfo_code_for_winner_next_seat = -69 ;
const int global_voteinfo_code_for_begin_tied_for_next_seat = -70 ;
const int global_voteinfo_code_for_end_tied_for_next_seat = -71 ;
const int global_voteinfo_code_for_counting_cycle_number = -72 ;
const int global_voteinfo_code_for_pairwise_losing_candidate = -73 ;
const int global_voteinfo_code_for_eliminated_candidate = -74 ;
const int global_voteinfo_code_for_quota_count_this_cycle = -75 ;
const int global_voteinfo_code_for_candidate_and_transfer_count = -76 ;
const int global_voteinfo_code_for_candidate_to_ignore = -77 ;
const int global_voteinfo_code_for_request_ignore_shared_rankings = -78 ;


// -----------------------------------------------
//  Declare an output file for writing log info.

std::ofstream log_out ;


// -----------------------------------------------
//  Declare an output file for writing spreadsheet
//  data.

std::ofstream spreadsheet_out ;


// -----------------------------------------------
//  Declare an input file for reading calculated
//  results.

std::ifstream calc_results ;


// -----------------------------------------------
//  Declare the random number generator.

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator ( seed ) ;


// -----------------------------------------------
//  End of global declarations.


// -----------------------------------------------
// -----------------------------------------------
//
//      do_initialization

void do_initialization( )
{

    int sequence_position ;
    int candidate_number ;
    int case_type ;
    int ranking_level ;
    int sub_ranking_level ;
    int ballot_group_number ;
    int verified_candidate_count ;
    int next_available_case_type_number ;
    int seats_to_fill ;
    int yes_or_no_switch_to_droop_quota ;
    int yes_or_no_eliminate_pairwise_losing_candidate ;
    int yes_or_no_request_ignore_shared_rankings ;


// -----------------------------------------------
//  Here's some unique text -- "settings_here" --
//  to use to quickly find the settings.


// -----------------------------------------------
//  Specify how many cases to test.  Each case
//  calculates election results for just one set
//  of conditions.  New ballots are only generated
//  when the case type is the first case type,
//  which corresponds to case type 1.

    global_case_count_limit = 500000 ;
//    global_case_count_limit = 200 ;


// -----------------------------------------------
//  Specify ID numbers and characteristics for the
//  different case types.  Specifically, specify
//  how many seats need to be filled for each
//  type, and whether to eliminate pairwise losing
//  candidates, and whether to use the Droop quota
//  instead of the Hare quota, and whether to
//  also compare with primitive STV that ignores
//  ballots with any shared preference levels.
//  The first case type must be VoteFair
//  representation ranking.

//  VoteFair representation ranking, 2 seats
    next_available_case_type_number = 1 ;
    global_case_type_votefair_representation = next_available_case_type_number ;
    global_number_of_seats_to_fill_for_case_type[ global_case_type_votefair_representation ] = 2 ;
    global_yes_or_no_eliminate_pairwise_losing_candidate_for_case_type[ global_case_type_votefair_representation ] = global_no ;
    global_yes_or_no_switch_to_droop_quota_for_case_type[ global_case_type_votefair_representation ] = global_no ;
    global_yes_or_no_request_ignore_shared_rankings_for_case_type[ global_case_type_votefair_representation ] = global_no ;
    global_count_of_successful_cases_for_case_type[ global_case_type_votefair_representation ] = 0 ;

//  VoteFair popularity ranking, 1 seat
    next_available_case_type_number ++ ;
    global_case_type_votefair_popularity = next_available_case_type_number ;
    global_number_of_seats_to_fill_for_case_type[ global_case_type_votefair_popularity ] = 1 ;
    global_yes_or_no_eliminate_pairwise_losing_candidate_for_case_type[ global_case_type_votefair_popularity ] = global_no ;
    global_yes_or_no_switch_to_droop_quota_for_case_type[ global_case_type_votefair_popularity ] = global_no ;
    global_yes_or_no_request_ignore_shared_rankings_for_case_type[ global_case_type_votefair_popularity ] = global_no ;
    global_count_of_successful_cases_for_case_type[ global_case_type_votefair_popularity ] = 0 ;


//  Loop through the STV and RCIPE STV variations.

    for ( yes_or_no_switch_to_droop_quota = global_no ; yes_or_no_switch_to_droop_quota <= global_yes ; yes_or_no_switch_to_droop_quota ++ )
    {
        for ( yes_or_no_request_ignore_shared_rankings = global_no ; yes_or_no_request_ignore_shared_rankings <= global_yes ; yes_or_no_request_ignore_shared_rankings ++ )
        {
            for ( yes_or_no_eliminate_pairwise_losing_candidate = global_no ; yes_or_no_eliminate_pairwise_losing_candidate <= global_yes ; yes_or_no_eliminate_pairwise_losing_candidate ++ )
            {
                if ( ( yes_or_no_eliminate_pairwise_losing_candidate == global_yes ) && ( yes_or_no_request_ignore_shared_rankings == global_yes ) )
                {
                    continue ;
                }
                for ( seats_to_fill = 1 ; seats_to_fill <= 5 ; seats_to_fill ++ )
                {
                    next_available_case_type_number ++ ;
                    global_number_of_seats_to_fill_for_case_type[ next_available_case_type_number ] = seats_to_fill ;
                    global_yes_or_no_eliminate_pairwise_losing_candidate_for_case_type[ next_available_case_type_number ] = yes_or_no_eliminate_pairwise_losing_candidate ;
                    global_yes_or_no_switch_to_droop_quota_for_case_type[ next_available_case_type_number ] = yes_or_no_switch_to_droop_quota ;
                    global_yes_or_no_request_ignore_shared_rankings_for_case_type[ next_available_case_type_number ] = yes_or_no_request_ignore_shared_rankings ;
                    global_count_of_successful_cases_for_case_type[ next_available_case_type_number ] = 0 ;
                }
            }
        }
    }

    global_number_of_case_types_to_test = next_available_case_type_number ;


// -----------------------------------------------
//  Specify which case types to test.  This use of
//  a pointer to the case types allows a few
//  specific case types to be tested, without
//  testing all the case types.
//
//  Reminder:  Case type 1 must always be:
//  global_case_type_votefair_representation

    for ( case_type = 1 ; case_type <= global_number_of_case_types_to_test ; case_type ++ )
    {
        global_list_of_case_types_to_test[ case_type ] = case_type ;
    }


// -----------------------------------------------
//  Specify the plurality method as a special case
//  type that does not involve external counting
//  because the vote counting is done internally.
//  This case type is added as an extra that is
//  not included in the loops.

    global_case_type_plurality = global_number_of_case_types_to_test + 1 ;
    global_number_of_seats_to_fill_for_case_type[ global_case_type_plurality ] = 1 ;
    global_yes_or_no_eliminate_pairwise_losing_candidate_for_case_type[ global_case_type_plurality ] = global_no ;
    global_yes_or_no_switch_to_droop_quota_for_case_type[ global_case_type_plurality ] = global_no ;
    global_yes_or_no_request_ignore_shared_rankings_for_case_type[ global_case_type_plurality ] = global_yes ;
    global_count_of_successful_cases_for_case_type[ global_case_type_plurality ] = 0 ;
    global_list_of_case_types_to_test[ global_case_type_plurality ] = global_case_type_plurality ;


// -----------------------------------------------
//  Specify "random" as another special case.  It
//  always identifies candidate 1 as the winner.

    global_case_type_random = global_number_of_case_types_to_test + 2 ;
    global_number_of_seats_to_fill_for_case_type[ global_case_type_random ] = 1 ;
    global_yes_or_no_eliminate_pairwise_losing_candidate_for_case_type[ global_case_type_random ] = global_no ;
    global_yes_or_no_switch_to_droop_quota_for_case_type[ global_case_type_random ] = global_no ;
    global_yes_or_no_request_ignore_shared_rankings_for_case_type[ global_case_type_random ] = global_yes ;
    global_count_of_successful_cases_for_case_type[ global_case_type_random ] = 0 ;
    global_list_of_case_types_to_test[ global_case_type_random ] = global_case_type_random ;


// -----------------------------------------------
//  Verify that the first case type is for
//  VoteFair representation ranking.  This is
//  needed because some code assumes this is true.

    if ( global_list_of_case_types_to_test[ 1 ] != global_case_type_votefair_representation )
    {
        log_out << "[Error: The first case type is not VoteFair representation ranking, which is assumed by some of the code]" << std::endl ;
        std::cout << "Error: The first case type is not VoteFair representation ranking, which is assumed by some of the code." << std::endl ;
        exit( EXIT_FAILURE ) ;
    }


// -----------------------------------------------
//  Specify the number of ballots in each ballot
//  group.  All the ballots in a ballot group have
//  the same (or equivalent) ballot-marking
//  pattern.

    global_maximum_ballot_group = 8 ;

    global_repeat_count_for_ballot_group[ 1 ] = 4 ;
    global_repeat_count_for_ballot_group[ 2 ] = 3 ;
    global_repeat_count_for_ballot_group[ 3 ] = 3 ;
    global_repeat_count_for_ballot_group[ 4 ] = 2 ;
    global_repeat_count_for_ballot_group[ 5 ] = 2 ;
    global_repeat_count_for_ballot_group[ 6 ] = 1 ;
    global_repeat_count_for_ballot_group[ 7 ] = 1 ;
    global_repeat_count_for_ballot_group[ 8 ] = 1 ;


// -----------------------------------------------
//  Calculate the number of ballots.

    global_total_ballot_count = 0 ;
    for ( ballot_group_number = 1 ; ballot_group_number <= global_maximum_ballot_group ; ballot_group_number ++ )
    {
        global_total_ballot_count += global_repeat_count_for_ballot_group[ ballot_group_number ] ;
    }


// -----------------------------------------------
//  Specify the number of candidates.

    global_full_candidate_count = 11 ;


// -----------------------------------------------
//  Specify the number of ranking levels.

    global_maximum_ranking_level = 6 ;


// -----------------------------------------------
//  Calculate the number of ballots times the
//  candidate count minus one.

    global_ballot_count_times_candidate_count_minus_one = global_total_ballot_count * ( global_full_candidate_count - 1 ) ;


// -----------------------------------------------
//  Ensure that the case numbers have the same
//  number of digits.  This number can be changed.

    global_minimum_case_id = 100000 ;


// -----------------------------------------------
//  Specify whether to suppress writing details
//  to the log file.  This suppression is useful
//  when lots of cases are being run, and
//  debugging is not needed.

    global_yes_or_no_show_details_in_log_file = global_yes ;
//     global_yes_or_no_show_details_in_log_file = global_no ;


// -----------------------------------------------
//  Initialize global counts.

    global_test_count = 0 ;
    global_count_of_cases_ignored = 0 ;
    global_counter_for_ballot_pattern_shared_rankings = 1 ;


// -----------------------------------------------
//  Initialize the usage counters that keep track
//  of how many times each candidate was randomly
//  placed in each sequence position.

    for ( candidate_number = 1 ; candidate_number <= global_full_candidate_count ; candidate_number ++ )
    {
        for ( sequence_position = 1 ; sequence_position <= global_full_candidate_count ; sequence_position ++ )
        {
            global_usage_count_for_candidate_and_sequence_position[ candidate_number ][ sequence_position ] = 0 ;
        }
    }


// -----------------------------------------------
//  End of subroutine do_initialization.

}


// -----------------------------------------------
// -----------------------------------------------
//
//  convert_integer_to_text
//
//  This function is used instead of "std::to_string"
//  for compatibility with older C++ "string" libraries
//  that have a bug.  The bug is that the "to_string"
//  function is not recognized as being within the
//  "std" library, even though it is defined there.

std::string convert_integer_to_text( int supplied_integer )
{
    int unused_string_length ;
    char c_format_string[ 50 ] ;
    try
    {
        unused_string_length = sprintf( c_format_string , "%1d" , supplied_integer ) ;
        return ( std::string ) c_format_string ;
    }
    catch( ... )
    {
        return "NAN" ;
    }
}


// -----------------------------------------------
// -----------------------------------------------
//
//  convert_text_to_integer
//
//  This function is used instead of "std::to_string"
//  for compatibility with older C++ "string" libraries
//  that have a bug.  The bug is that the "to_string"
//  function is not recognized as being within the
//  "std" library, even though it is defined there.

int convert_text_to_integer( char * supplied_text )
{
    int equivalent_integer ;
    try
    {
        equivalent_integer = atoi( supplied_text ) ;
    }
    catch( ... )
    {
        equivalent_integer = -99999 ;
    }
    return equivalent_integer ;
}


// -----------------------------------------------
// -----------------------------------------------
//  convert_float_to_text
//
//  To read why this function is here, see the comment
//  above for function: convert_integer_to_text

std::string convert_float_to_text( float supplied_float )
{
    std::string returned_string ;
    char c_format_string[ 50 ] ;
    int unused_string_length ;
    try
    {
        unused_string_length = sprintf( c_format_string , "%1f" , supplied_float ) ;
        returned_string = ( std::string ) c_format_string ;
        //  next line assumes the sprintf result always includes a decimal point
        returned_string.erase( returned_string.find_last_not_of( "0" ) + 1 , std::string::npos ) ;
        returned_string.erase( returned_string.find_last_not_of( "." ) + 1 , std::string::npos ) ;
        return returned_string ;
    }
    catch( ... )
    {
        return "NAN" ;
    }
}


// -----------------------------------------------
// -----------------------------------------------
//
//     calculate_satisfaction
//
//  Calculate the satisfaction percent for the
//  identified seat winners.
//
//  If all the seat winners are ranked as first
//  choice by an equal number of voters, the
//  satisfaction rate is 100 percent.  As an
//  example of this full satisfaction value,
//  imagine there are three seats, one third of
//  the voters rank the first-seat winner as their
//  first choice, another third of the voters rank
//  the second-seat winner as their first choice,
//  and the remaining third of the voters rank
//  the third-seat winner as their first choice.
//
//  The degree of satisfaction for a voter is
//  measured by the number of candidates who are
//  ranked below the seat winner.  If all the
//  other candidates are ranked below the seat
//  winner, that ballot has full -- 100 percent
//  -- satisfaction.
//
//  A ballot is regarded as supporting a seat
//  winner to the degree it highly ranks that
//  seat winner.
//
//  Some ballots support more than one winner
//  because a supporting vote count beyond the
//  quota (needed to win) leaves a portion of the
//  ballot's influence available during the
//  filling of other seats.
//
//
// -----------------------------------------------

void calculate_satisfaction( ) {

    int candidate_number ;
    int sequence_position ;
    int ranking_level ;
    int ballot_group_number ;
    int ballot_group_repeat_count ;
    int pointer_to_supporting_ballot_group ;
    int supporting_ballot_group ;
    int number_of_supporting_ballot_groups ;
    int seat_number ;
    int candidate_seat_winner ;
    int proportional_satisfaction_percent_for_current_case ;
    int count_of_candidates_ranked_below_seat_winner ;
    int number_of_candidates_at_current_ranking_level ;

    float decimal_supporting_vote_count_weighted ;
    float decimal_count_of_supporting_ballots ;
    float decimal_influence_reduction_per_ballot ;
    float decimal_votes_that_must_support_seat_winner ;
    float decimal_influence_reduction_for_current_ballot_group ;
    float decimal_proportional_satisfaction_number_for_current_case ;


// -----------------------------------------------
//  Initialization.

    decimal_proportional_satisfaction_number_for_current_case = 0.0 ;
    for ( ballot_group_number = 1 ; ballot_group_number <= global_maximum_ballot_group ; ballot_group_number ++ )
    {
        ballot_group_repeat_count = global_repeat_count_for_ballot_group[ ballot_group_number ] ;
        global_decimal_remaining_influence_for_ballot_group[ ballot_group_number ] = float( ballot_group_repeat_count ) ;
        global_decimal_reduced_influence_for_ballot_group[ ballot_group_number ] = 0 ;
    }


// -----------------------------------------------
//  Begin a loop that handles each ranking level
//
//  Reminder: Ranking level 1 is the highest, not
//  the lowest, ranking level.

    log_out << std::endl ;
    for ( ranking_level = 1 ; ranking_level <= global_maximum_ranking_level ; ranking_level ++ )
    {


// -----------------------------------------------
//  Begin a loop that handles each seat winner.
//
//  Reminder: The winner of VoteFair popularity
//  ranking is always the same as the winner of
//  the first seat in two-seat VoteFair
//  representation ranking.

        for ( seat_number = 1 ; seat_number <= global_number_of_seats_to_fill_for_case_type[ global_case_type ] ; seat_number ++ )
        {
            candidate_seat_winner = 0 ;
            if ( ( global_case_type == global_case_type_votefair_popularity ) || ( global_case_type == global_case_type_votefair_representation ) )
            {
                candidate_seat_winner = global_candidate_who_won_votefair_seat_number[ seat_number ] ;
            } else if ( global_case_type == global_case_type_plurality )
            {
                candidate_seat_winner = global_plurality_winner ;
            } else if ( global_case_type == global_case_type_random )
            {
                candidate_seat_winner = 1 ;
            } else
            {
                candidate_seat_winner = global_candidate_who_won_seat_number[ seat_number ] ;
            }


// -----------------------------------------------
//  Exit this function if any of the seat winner
//  numbers are zero.  This situation indicates
//  that not all the seats were filled, which
//  means a satisfaction rate cannot be
//  calculated.

            if ( candidate_seat_winner == 0 )
            {
                decimal_proportional_satisfaction_number_for_current_case = 0.0 ;
                log_out << "[seat winner is zero, proportional satisfaction cannot be calculated]" << std::endl ;
                return ;
            }


// -----------------------------------------------
//  Initialize the counts that track how many
//  ballots, and which ballots, support the
//  current seat winner.

            decimal_supporting_vote_count_weighted = 0.0 ;
            decimal_count_of_supporting_ballots = 0 ;
            number_of_supporting_ballot_groups = 0 ;
            pointer_to_supporting_ballot_group = 0 ;


// -----------------------------------------------
//  Begin a loop that handles each ballot group.

            for ( ballot_group_number = 1 ; ballot_group_number <= global_maximum_ballot_group ; ballot_group_number ++ )
            {
                ballot_group_repeat_count = global_repeat_count_for_ballot_group[ ballot_group_number ] ;


// -----------------------------------------------
//  Within this ballot group (of identical
//  ballots), if the seat winner is not ranked at
//  the ranking level that's currently of
//  interest, skip this ballot group.

                if ( global_ranking_for_candidate_and_ballot_group[ candidate_seat_winner ][ ballot_group_number ] != ranking_level )
                {
                    continue ;
                }


// -----------------------------------------------
//  Count how many candidates are ranked at the
//  current ranking level.
//  Reminder: The highest preference level is one.

                number_of_candidates_at_current_ranking_level = 0 ;
                for ( candidate_number = 1 ; candidate_number <= global_full_candidate_count ; candidate_number ++ )
                {
                    if ( global_ranking_for_candidate_and_ballot_group[ candidate_number ][ ballot_group_number ] == ranking_level )
                    {
                        number_of_candidates_at_current_ranking_level ++ ;
                    }
                }


// -----------------------------------------------
//  Within this ballot group, calculate how many
//  votes support this seat-winning candidate,
//  allowing for the possibility that these
//  ballots have more than one candidate ranked at
//  the same ranking level (of interest).

                decimal_supporting_vote_count_weighted = global_decimal_remaining_influence_for_ballot_group[ ballot_group_number ] / float( number_of_candidates_at_current_ranking_level ) ;

                global_decimal_reduced_influence_for_ballot_group[ ballot_group_number ] = decimal_supporting_vote_count_weighted ;

                log_out << "[rank " << ranking_level << ", seat winner " << candidate_seat_winner << ", group " << ballot_group_number << ", repeat count " << ballot_group_repeat_count << ", remaining " << convert_float_to_text( global_decimal_remaining_influence_for_ballot_group[ ballot_group_number ] ) << ", support " << convert_float_to_text( decimal_supporting_vote_count_weighted ) << "]" << std::endl ;


// -----------------------------------------------
//  If this ballot group provided any support to
//  this seat winner, add that amount to the vote
//  count supporting this seat winner.  Also track
//  the number of supporting ballots.  Also add
//  the ballot group number to the list that
//  tracks the supporting ballots.

                if ( decimal_supporting_vote_count_weighted > 0.0 )
                {
                    decimal_count_of_supporting_ballots += global_decimal_remaining_influence_for_ballot_group[ ballot_group_number ] ;
                    pointer_to_supporting_ballot_group ++ ;
                    global_list_of_supporting_ballot_groups[ pointer_to_supporting_ballot_group ] = ballot_group_number ;
                    number_of_supporting_ballot_groups = pointer_to_supporting_ballot_group ;
                }


// -----------------------------------------------
//  Repeat the loop to handle the next ballot
//  group.

            }


// -----------------------------------------------
//  If none of the ballots -- at the specified
//  ranking level -- support the specified winning
//  candidate, repeat the loop to handle the next
//  winning candidate.

            if ( decimal_count_of_supporting_ballots <= 0.0 )
            {
//                log_out << "[no support for winning candidate " << candidate_seat_winner << " at ranking level " << ranking_level << "]" << std::endl ;
                continue ;
            }


// -----------------------------------------------
//  Begin a loop that handles each group of
//  supporting ballots.

            if ( number_of_supporting_ballot_groups > 0 )
            {
                for ( pointer_to_supporting_ballot_group = 1 ; pointer_to_supporting_ballot_group <= number_of_supporting_ballot_groups ; pointer_to_supporting_ballot_group ++ )
                {
                    supporting_ballot_group = global_list_of_supporting_ballot_groups[ pointer_to_supporting_ballot_group ] ;


// -----------------------------------------------
//  Reduce the influence of the supporting ballot
//  group.  This reduction leaves less influence
//  toward electing further winners.  This
//  reduction is the same as the amount of support
//  that was just calculated.

                    global_decimal_remaining_influence_for_ballot_group[ supporting_ballot_group ] -= global_decimal_reduced_influence_for_ballot_group[ supporting_ballot_group ] ;
                    log_out << "[group " << supporting_ballot_group << ", reduced by " << convert_float_to_text( global_decimal_reduced_influence_for_ballot_group[ supporting_ballot_group ] ) << ", reduced to " << convert_float_to_text( global_decimal_remaining_influence_for_ballot_group[ supporting_ballot_group ] )  << "]" << std::endl ;


// -----------------------------------------------
//  Count how many candidates are ranked below the
//  current seat winner.
//  Reminder: The highest preference level is one.

                    count_of_candidates_ranked_below_seat_winner = 0 ;
                    for ( candidate_number = 1 ; candidate_number <= global_full_candidate_count ; candidate_number ++ )
                    {
                        if ( global_ranking_for_candidate_and_ballot_group[ candidate_number ][ supporting_ballot_group ] > global_ranking_for_candidate_and_ballot_group[ candidate_seat_winner ][ supporting_ballot_group ] )
                        {
                            count_of_candidates_ranked_below_seat_winner ++ ;
                        }
                    }


// -----------------------------------------------
//  For each of the supporting ballot groups, add
//  the supporting influence amount to the total
//  proportional satisfaction number.

                    sequence_position = global_sequence_position_for_candidate_and_ballot_group[ candidate_seat_winner ][ supporting_ballot_group ] ;

                    decimal_proportional_satisfaction_number_for_current_case += ( global_decimal_reduced_influence_for_ballot_group[ supporting_ballot_group ] * float( count_of_candidates_ranked_below_seat_winner ) ) ;


// -----------------------------------------------
//  Repeat the branch and loop that handles the
//  next supporting ballot group.

                }
            }


// -----------------------------------------------
//  Repeat the loop to handle the next seat
//  winner.

        }


// -----------------------------------------------
//  Repeat the loop to handle the next ranking
//  level.

    }


// -----------------------------------------------
//  Show any remaining influence.

    for ( ballot_group_number = 1 ; ballot_group_number <= global_maximum_ballot_group ; ballot_group_number ++ )
    {
        if ( global_decimal_remaining_influence_for_ballot_group[ ballot_group_number ] > 0.0 )
        {
            log_out << "[remaining influence in group " << ballot_group_number << " is " << convert_float_to_text( global_decimal_remaining_influence_for_ballot_group[ ballot_group_number ] )  << " votes]" << std::endl ;
        }
    }


// -----------------------------------------------
//  Calculate, and log, the satisfaction rate.

//    log_out << "[satisfaction count is " << convert_float_to_text( decimal_proportional_satisfaction_number_for_current_case ) << "]" << std::endl ;

    proportional_satisfaction_percent_for_current_case = int( 100.0 * decimal_proportional_satisfaction_number_for_current_case / float( global_ballot_count_times_candidate_count_minus_one ) ) ;

    log_out << std::endl << "[satisfaction percent for case type " << global_case_type << " is " << proportional_satisfaction_percent_for_current_case << "]" << std::endl ;


// -----------------------------------------------
//  Count the number of satisfaction calculations
//  done for the current case type.

    global_count_of_successful_cases_for_case_type[ global_case_type ] ++ ;


// -----------------------------------------------
//  Write data to a spreadsheet-like file that
//  allows standard deviation values and ranges to
//  be calculated.
//
//  Reminder: Use "sample" standard deviation
//  not "population" standard deviation because
//  sampling is involved.  Divisor is N-1 instead
//  of N.
//  In Libre Office use: =STDEV.P

    spreadsheet_out << global_case_type << " " << proportional_satisfaction_percent_for_current_case << std::endl ;


// -----------------------------------------------
//  End of function calculate_satisfaction

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//
//  generate_ballots
//
//  This function generates random preferences
//  for the ballots, and generates the ballot
//  data in a format that is supplied to the
//  software that calculates election winners.

void generate_ballots( ) {

    int ballot_group_number ;
    int ballot_group_repeat_count ;
    int candidate_number ;
    int candidate_at_current_sequence_position ;
    int candidate_at_first_sequence_position ;
    int sequence_position ;
    int picking_position ;
    int count_of_candidates_not_yet_ranked ;
    int ranking_level ;
    int pointer_number ;
    int highest_plurality_count ;
    int saved_case_type ;
    int yes_or_no_insert_tie_here ;


// -----------------------------------------------
//  Initialization.

    log_out << "[new ballots]" ;
    global_pointer_to_list_voteinfo_output_ballot = 0 ;
    for ( candidate_number = 1 ; candidate_number <= global_full_candidate_count ; candidate_number ++ )
    {
        global_count_first_choice_usage_of_candidate[ candidate_number ] = 0 ;
    }


// -----------------------------------------------
//  Count the number of times new ballots are
//  generated.

    global_count_ballot_generation_done ++ ;


// -----------------------------------------------
//  Begin a loop that handles each ballot group.

    for ( ballot_group_number = 1 ; ballot_group_number <= global_maximum_ballot_group ; ballot_group_number ++ )
    {


// -----------------------------------------------
//  Specify how many ballots have this pattern.

        ballot_group_repeat_count = global_repeat_count_for_ballot_group[ ballot_group_number ] ;


// -----------------------------------------------
//  Specify the beginning of the info for this
//  ballot group.

        global_pointer_to_list_voteinfo_output_ballot ++ ;
        global_list_voteinfo_output_ballot[ global_pointer_to_list_voteinfo_output_ballot ] = global_voteinfo_code_for_ballot_count ;
        global_pointer_to_list_voteinfo_output_ballot ++ ;
        global_list_voteinfo_output_ballot[ global_pointer_to_list_voteinfo_output_ballot ] = ballot_group_repeat_count ;


// -----------------------------------------------
//  Specify question number one.  This is needed
//  for other software, especially
//  votefair_ranking.cpp, that can handle multiple
//  questions in the same ballot group.)

        global_pointer_to_list_voteinfo_output_ballot ++ ;
        global_list_voteinfo_output_ballot[ global_pointer_to_list_voteinfo_output_ballot ] = global_voteinfo_code_for_question_number ;
        global_pointer_to_list_voteinfo_output_ballot ++ ;
        global_list_voteinfo_output_ballot[ global_pointer_to_list_voteinfo_output_ballot ] = 1 ;


// -----------------------------------------------
//  Choose to sometimes -- currently set as one
//  out of three ballot groups -- rank the
//  second-chosen candidate to be ranked at the
//  "first choice" level, which means that
//  sometimes two candidates are ranked at the top
//  preference level.

        global_counter_for_ballot_pattern_shared_rankings ++ ;
        if ( global_counter_for_ballot_pattern_shared_rankings > 3 )
        {
            global_counter_for_ballot_pattern_shared_rankings = 1 ;
        }


// -----------------------------------------------
//  Put the candidate numbers into a "picking"
//  list so they can be chosen at random without
//  repeating any candidate number.

        for ( candidate_number = 1 ; candidate_number <= global_full_candidate_count ; candidate_number ++ )
        {
            global_candidate_number_at_picking_position[ candidate_number ] = candidate_number ;
        }


// -----------------------------------------------
//  Begin a loop that chooses each candidate in
//  the ballot's sequence.

        count_of_candidates_not_yet_ranked = global_full_candidate_count ;
        ranking_level = 1 ;
        for ( sequence_position = 1 ; sequence_position <= global_full_candidate_count ; sequence_position ++ )
        {


// -----------------------------------------------
//  Randomly choose a candidate number for this
//  sequence position.  Do not repeat any
//  candidate number already used at a previous
//  sequence position.

            std::uniform_int_distribution<int> distribution( 1 , count_of_candidates_not_yet_ranked ) ;
            picking_position = distribution( generator ) ;
            candidate_at_current_sequence_position = global_candidate_number_at_picking_position[ picking_position ] ;
            global_sequence_position_for_candidate_and_ballot_group[ candidate_at_current_sequence_position ][ ballot_group_number ] = sequence_position ;


// -----------------------------------------------
//  Keep track of candidate number usage to allow
//  verifying randomness.

            global_usage_count_for_candidate_and_sequence_position[ candidate_at_current_sequence_position ][ sequence_position ] ++ ;


// -----------------------------------------------
//  Update the list of which candidates still need
//  to be assigned to a sequence position.

            if ( picking_position < count_of_candidates_not_yet_ranked )
            {
                for ( pointer_number = picking_position ; pointer_number <= ( global_full_candidate_count - 1 ) ; pointer_number ++ )
                {
                    global_candidate_number_at_picking_position[ pointer_number ] = global_candidate_number_at_picking_position[ pointer_number + 1 ] ;
                }
            }


// -----------------------------------------------
//  Insert a tie if this candidate should share
//  the same ranking level as the previous
//  candidate.  This characteristic is specified
//  to occur randomly, yet not too often at the
//  "first choice" ranking level.

//  todo: test new code here

            if ( sequence_position == 1 )
            {
                yes_or_no_insert_tie_here = global_no ;
            } else if ( sequence_position == 2 )
            {
                if ( global_counter_for_ballot_pattern_shared_rankings != 3 )
                {
                    yes_or_no_insert_tie_here = global_no ;
                } else
                {
                    yes_or_no_insert_tie_here = global_yes ;
                }
            } else if ( ( ranking_level == global_maximum_number_of_ranking_levels ) && ( sequence_position < global_maximum_number_of_candidates ) )
            {
                yes_or_no_insert_tie_here = global_yes ;
            } else
            {
                if ( yes_or_no_insert_tie_here == global_yes )
                {
                    yes_or_no_insert_tie_here = global_no ;
                } else
                {
                    yes_or_no_insert_tie_here = global_yes ;
                }
            }
            if ( yes_or_no_insert_tie_here == global_yes )
            {
                global_pointer_to_list_voteinfo_output_ballot ++ ;
                global_list_voteinfo_output_ballot[ global_pointer_to_list_voteinfo_output_ballot ] = global_voteinfo_code_for_tie ;
            }


// -----------------------------------------------
//  If only one candidate is marked at the "first
//  choice" ranking level, count how many ballots
//  on which it is the first choice.

            if ( sequence_position == 1 )
            {
                candidate_at_first_sequence_position = candidate_at_current_sequence_position ;
            } else if ( ( sequence_position == 2 ) && ( yes_or_no_insert_tie_here == global_no ) )
            {
                global_count_first_choice_usage_of_candidate[ candidate_at_first_sequence_position ] += ballot_group_repeat_count ;
//                log_out << std::endl << "[at ballot group " << ballot_group_number << " candidate " << candidate_at_first_sequence_position << " now has plurality count " << global_count_first_choice_usage_of_candidate[ candidate_at_first_sequence_position ] << "]" << std::endl ;
            }


// -----------------------------------------------
//  Insert the just-picked candidate into the
//  ballot's preference sequence.  This candidate
//  might be ranked at the same ranking level as
//  the previous candidate, so the ranking level
//  does not necessarily match the sequence
//  position.

            global_pointer_to_list_voteinfo_output_ballot ++ ;
            global_list_voteinfo_output_ballot[ global_pointer_to_list_voteinfo_output_ballot ] = candidate_at_current_sequence_position ;


// -----------------------------------------------
//  Keep track of the ranking info for use later
//  when calculating the satisfaction percent.

            global_ranking_for_candidate_and_ballot_group[ candidate_at_current_sequence_position ][ ballot_group_number ] = ranking_level ;
            if ( yes_or_no_insert_tie_here == global_no )
            {
                ranking_level ++ ;
            }


// -----------------------------------------------
//  Repeat the loop that handles each sequence
//  position.

            count_of_candidates_not_yet_ranked -- ;
        }


// -----------------------------------------------
//  Write the code that terminates this ballot.

        global_pointer_to_list_voteinfo_output_ballot ++ ;
        global_list_voteinfo_output_ballot[ global_pointer_to_list_voteinfo_output_ballot ] = global_voteinfo_code_for_end_of_ballot ;


// -----------------------------------------------
//  Repeat the loop that handles each ballot
//  group.

    }


// -----------------------------------------------
//  Identify the plurality winner, which is the
//  candidate who is ranked first on the most
//  ballots.

    global_plurality_winner = 0 ;
    highest_plurality_count = 0 ;
    for ( candidate_number = 1 ; candidate_number <= global_full_candidate_count ; candidate_number ++ )
    {
        if ( (  global_count_first_choice_usage_of_candidate[ candidate_number ] > highest_plurality_count ) || (  highest_plurality_count == 0 ) )
        {
            highest_plurality_count = global_count_first_choice_usage_of_candidate[ candidate_number ] ;
            global_plurality_winner = candidate_number ;
        } else if ( global_count_first_choice_usage_of_candidate[ candidate_number ] == highest_plurality_count )
        {
            global_plurality_winner = 0 ;
        }
    }
    if ( global_plurality_winner > 0 )
    {
        log_out << std::endl << "[candidate " << global_plurality_winner << " is plurality winner with count " << highest_plurality_count << "]" << std::endl ;
    } else
    {
        log_out << std::endl << "[there is no plurality winner]" << std::endl ;
    }


// -----------------------------------------------
//  Determine whether the plurality winner is, or
//  is not, supported by a majority of voters.

    global_majority_winner = 0 ;
    if ( global_plurality_winner > 0 )
    {
        if ( highest_plurality_count >= 1 + int( float( global_total_ballot_count ) / 2.0 ) )
        {
            global_majority_winner = global_plurality_winner ;
            log_out << "[candidate " << global_majority_winner << " is first-choice majority winner]" << std::endl ;
        } else
        {
            log_out << "[there is no first-choice majority winner]" << std::endl ;
        }
    }


// -----------------------------------------------
//  If there is a plurality winner, calculate and
//  write the proportional satisfaction rate for
//  that candidate getting elected.

    if ( global_plurality_winner > 0 )
    {
        log_out << "[calculating proportional satisfaction rate for plurality winner]" << std::endl ;
        saved_case_type = global_case_type ;
        global_case_type = global_case_type_plurality ;
        calculate_satisfaction( ) ;
        global_case_type = saved_case_type ;
    }


// -----------------------------------------------
//  Calculate and write the proportional
//  satisfaction rate for the "random" method,
//  which always elects candidate 1.

    log_out << std::endl << "[candidate 1 is random winner]" << std::endl << "[calculating proportional satisfaction rate for random winner]" << std::endl ;
    saved_case_type = global_case_type ;
    global_case_type = global_case_type_random ;
    calculate_satisfaction( ) ;
    global_case_type = saved_case_type ;


// -----------------------------------------------
//  End of function generate_ballots.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//
//     handle_calculated_results
//
//  Reads numbers and codes from the file
//  written by the calculation program.
//
// -----------------------------------------------

void handle_calculated_results( )
{

    int candidate_number ;
    int ballot_group_repeat_count ;
    int position_pointer ;
    int current_result_code ;
    int previous_result_code ;
    int next_result_code ;
    int count_of_result_codes ;
    int seat_number ;
    int do_nothing ;

    std::string input_line ;
    std::string input_text_word ;


// -----------------------------------------------
//  Initialization.

    current_result_code = 0 ;
    previous_result_code = 0 ;
    next_result_code = 0 ;
    count_of_result_codes = 0 ;
    for ( seat_number = 1 ; seat_number <= global_maximum_number_of_seats_to_fill ; seat_number ++ )
    {
        global_candidate_who_won_seat_number[ seat_number ] = 0 ;
        if ( global_case_type == 1 )
        {
            global_candidate_who_won_votefair_seat_number[ seat_number ] = 0 ;
        }
    }
    seat_number = 0 ;
    global_count_of_seats_filled = 0 ;
    global_candidate_winner = 0 ;
    do_nothing = 0 ;


// -----------------------------------------------
//  Begin a loop that handles each line from the
//  input file that contains the calculated
//  results.

    for ( std::string input_line ; std::getline( calc_results , input_line ) ; )
    {
        std::size_t pointer_found = input_line.find_last_not_of( " \t\n\r" ) ;
        if ( pointer_found != std::string::npos )
        {
            input_line.erase( pointer_found + 1 ) ;
        } else
        {
            input_line.clear( ) ;
        }

//        log_out << "[input line: " << input_line << "]" << std::endl ;

        char input_line_c_version[ 2000 ] = "" ;
        std::size_t line_length = std::min( 2000 , (int) input_line.length() ) ;
        std::size_t line_length_copied = input_line.copy( input_line_c_version , line_length , 0 ) ;
        input_line_c_version[ line_length_copied ] = '\0' ;


// -----------------------------------------------
//  Begin loop that handles each space-delimited
//  word of text from the input line.

        char * pointer_to_word ;
        // reminder: strtok modifies the string
        pointer_to_word = strtok( input_line_c_version , " ,." ) ;
        while ( pointer_to_word != NULL )
        {


// -----------------------------------------------
//  Attempt to convert the input word into an
//  integer.  If this conversion is not successful,
//  indicate an error and write this error to the
//  output file and then exit the program.

            try
            {
                current_result_code = convert_text_to_integer( pointer_to_word ) ;
            }
            catch( ... )
            {
                log_out << "[Warning: Input line from vote-counting software contains non-numeric characters (" << pointer_to_word << "), so this case (" << global_case_id << ") cannot be calculated]" << std::endl ;
                std::cout << "Warning: Input line from vote-counting software contains non-numeric characters (" << pointer_to_word << "), so this case (" << global_case_id << ") cannot be calculated." << std::endl ;
                exit( EXIT_FAILURE ) ;
            }
            count_of_result_codes ++ ;
            global_result_code_at_position[ count_of_result_codes ] = current_result_code ;
            global_result_code_at_prior_position[ 1 ] = current_result_code ;


// -----------------------------------------------
//  Identify interesting cases.

            if ( current_result_code == global_voteinfo_code_for_flag_as_interesting )
            {
                log_out << "[case " << global_case_id << " was flagged as interesting]" ;
            }


// -----------------------------------------------
//  Log interesting details.

            if ( current_result_code != global_voteinfo_code_for_number_of_equivalent_seats )
            {
                do_nothing ++ ;
            } else if ( previous_result_code != global_voteinfo_code_for_number_of_equivalent_seats )
            {
                do_nothing ++ ;
            } else if ( current_result_code != global_voteinfo_code_for_request_quota_droop_not_hare )
            {
                do_nothing ++ ;
            } else if ( current_result_code != global_voteinfo_code_for_request_ignore_shared_rankings )
            {
                log_out << "[ignore_shared_rankings]" ;
            } else if ( previous_result_code == global_voteinfo_code_for_winner_next_seat )
            {
                log_out << "[win " << current_result_code << "]" ;
            } else if ( current_result_code == global_voteinfo_code_for_begin_tied_for_next_seat )
            {
                log_out << "[begin_tied]" ;
            } else if ( current_result_code == global_voteinfo_code_for_end_tied_for_next_seat )
            {
                log_out << "[end_tied]" ;
            } else if ( current_result_code == global_voteinfo_code_for_pairwise_losing_candidate )
            {
                log_out << "[pairwise_loser]" ;
            } else if ( current_result_code == global_voteinfo_code_for_eliminated_candidate )
            {
                log_out << "[elim]" ;
            } else if ( current_result_code == global_voteinfo_code_for_request_instant_runoff_voting )
            {
                log_out << "[irv_no_ple]" ;
            } else if ( current_result_code == global_voteinfo_code_for_winner_next_seat )
            {
                do_nothing ++ ;
            } else if ( current_result_code == global_voteinfo_code_for_end_of_all_cases )
            {
                do_nothing ++ ;
            } else if ( current_result_code == 0 )
            {
                do_nothing ++ ;
            } else
            {
                log_out << "[" << current_result_code << "]" ;
            }


// -----------------------------------------------
//  From the RCIPE STV software, get the candidate
//  numbers for the seat winners.

            if ( ( global_case_type != global_case_type_votefair_popularity ) && ( global_case_type != global_case_type_votefair_representation ) && ( previous_result_code == global_voteinfo_code_for_winner_next_seat ) )
            {
                candidate_number = current_result_code ;
                global_count_of_seats_filled ++ ;
                global_candidate_who_won_seat_number[ global_count_of_seats_filled ] = candidate_number ;
                log_out << "[winner " << candidate_number << "]" ;
            }


// -----------------------------------------------
//  Get the most popular candidates according to
//  VoteFair Ranking.  The winner of the first
//  seat in VoteFair Representation ranking is
//  the same as VoteFair popularity ranking, which
//  yields the same winners as the Kemeny method.
//  The winner of the second seat is the candidate
//  who is most popular among the voters who are
//  not well-represented by the first-seat winner.

            if ( ( global_case_type == global_case_type_votefair_representation ) && ( global_result_code_at_prior_position[ 7 ] == global_voteinfo_code_for_start_of_votefair_representation_ranking_sequence_results ) )
            {
                if ( ( global_result_code_at_prior_position[ 7 ] == global_voteinfo_code_for_start_of_votefair_representation_ranking_sequence_results ) && ( global_result_code_at_prior_position[ 6 ] == global_voteinfo_code_for_choice ) && ( global_result_code_at_prior_position[ 4 ] == global_voteinfo_code_for_next_ranking_level ) && ( global_result_code_at_prior_position[ 3 ] == global_voteinfo_code_for_choice ) && ( global_result_code_at_prior_position[ 1 ] == global_voteinfo_code_for_next_ranking_level ) )
                {
                    global_candidate_who_won_votefair_seat_number[ 1 ] = global_result_code_at_prior_position[ 5 ] ;
                    global_candidate_who_won_votefair_seat_number[ 2 ] = global_result_code_at_prior_position[ 2 ] ;
                    log_out << "[VoteFair ranking seat winners " << global_candidate_who_won_votefair_seat_number[ 1 ] << " and " << global_candidate_who_won_votefair_seat_number[ 2 ] << "]" ;
                }
            }


// -----------------------------------------------
//  Save the most recent result codes, then repeat
//  the loop for the next word (within the line).

            for ( position_pointer = 9 ; position_pointer >= 1 ; position_pointer -- )
            {
                global_result_code_at_prior_position[ position_pointer + 1 ] = global_result_code_at_prior_position[ position_pointer ] ;
            }
            previous_result_code = current_result_code ;
            pointer_to_word = strtok( NULL, " ,." ) ;
        }


// -----------------------------------------------
//  Repeat the loop for the next line of data from
//  the input file.

    }


// -----------------------------------------------
//  End of function handle_calculated_results

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//
//     log_case_type_info
//
//  Log the settings for one case type.
//
//
// -----------------------------------------------

void log_case_type_info( ) {


// -----------------------------------------------
//  Write the case type settings to the log file.

    if ( global_number_of_seats_to_fill_for_case_type[ global_case_type ] == 1 )
    {
        log_out << "1 seat, " ;
    } else
    {
        log_out << global_number_of_seats_to_fill_for_case_type[ global_case_type ] << " seats, " ;
    }

    if ( global_case_type == global_case_type_votefair_popularity )
    {
        log_out << "VoteFair popularity" ;
    } else if ( global_case_type == global_case_type_votefair_representation )
    {
        log_out << "VoteFair representation" ;
    } else if ( global_case_type == global_case_type_plurality )
    {
        log_out << "Plurality" ;
    } else if ( global_case_type == global_case_type_random )
    {
        log_out << "Random" ;
    } else
    {
        if ( global_yes_or_no_eliminate_pairwise_losing_candidate_for_case_type[ global_case_type ] == global_yes )
        {
            log_out << "RCIPE" ;
        } else
        {
            log_out << "STV/IRV" ;
        }
        if ( global_yes_or_no_switch_to_droop_quota_for_case_type[ global_case_type ] == global_no )
        {
            log_out << ", hare" ;
        } else
        {
            log_out << ", droop" ;
        }
        if ( global_yes_or_no_request_ignore_shared_rankings_for_case_type[ global_case_type ] == global_yes )
        {
            log_out << ", shared rankings not counted" ;
        }
    }


// -----------------------------------------------
//  End of function log_case_type_info.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//
//     do_all_tests
//
//  Do all the tests using a specified number of
//  candidates.
//
// -----------------------------------------------

void do_all_tests( ) {

    int pointer_to_case_type ;
    int case_type ;
    int seat_number ;
    int seats_to_fill ;
    int sequence_position ;
    int candidate_number ;
    int ballot_group_number ;
    int ballot_group_repeat_count ;
    int integer_from_system_call ;
    int calculated_ppss ;
    int last_case_id ;
    int count_of_winners_matching ;
    int votefair_seat_number ;
    int stv_seat_number ;
    int candidate_number_rcipe ;
    int do_nothing_here ;
    int pointer_to_list_voteinfo ;
    int results_category_number ;
    int yes_or_no_wrote_category_name ;
    int ranking_level ;
    int yes_or_no_switch_to_droop_quota ;
    int yes_or_no_eliminate_pairwise_losing_candidate ;
    int yes_or_no_request_ignore_shared_rankings ;

    float proportional_satisfaction_percent ;

//    std::string case_id_as_text ;


// -----------------------------------------------
//  Start a new joined log file.

    integer_from_system_call = system( "del temp_joined_output_rcipe_stv_log.txt" ) ;


// -----------------------------------------------
//  Log the case types.

    pointer_to_case_type ++ ;
    for ( pointer_to_case_type = 1 ; pointer_to_case_type <= ( global_number_of_case_types_to_test + 2 ) ; pointer_to_case_type ++ )
    {
        global_case_type = global_list_of_case_types_to_test[ pointer_to_case_type ] ;
        log_out << "Case type " << global_case_type << ": " ;
        log_case_type_info( ) ;
        log_out << std::endl ;
    }


// -----------------------------------------------
//  Begin a loop that handles each case.
//  Each case sends ballot information to the
//  software that reads the ballot information and
//  identifies which candidates deserve to win
//  according to different vote-counting methods.

    pointer_to_case_type = 0 ;
    last_case_id = global_minimum_case_id + global_case_count_limit - 1 ;
    for ( global_case_id = global_minimum_case_id ; global_case_id <= last_case_id ; global_case_id ++ )
    {
        log_out << std::endl ;


// -----------------------------------------------
//  Cycle through the case types.

        pointer_to_case_type ++ ;
        if ( ( pointer_to_case_type > global_number_of_case_types_to_test ) || ( pointer_to_case_type < 1 ) || ( global_case_type < 1 ) || ( global_case_type > global_number_of_case_types_to_test ) )
        {
            pointer_to_case_type = 1 ;
        }
        global_case_type = global_list_of_case_types_to_test[ pointer_to_case_type ] ;


// -----------------------------------------------
//  Currently, skip doing the cases that involve
//  the Hare quota.

        if ( ( global_case_type != global_case_type_random ) && ( global_case_type != global_case_type_plurality ) &&  ( global_case_type != global_case_type_votefair_popularity ) && ( global_case_type != global_case_type_votefair_representation ) && ( global_yes_or_no_switch_to_droop_quota_for_case_type[ global_case_type ] == global_no ) )
        {
            log_out << "[Skipping Hare quota case type " << global_case_type << "]" << std::endl ;
            continue ;
        }


// -----------------------------------------------
//  Show progress on the screen.

        std::cout << global_case_type << " " ;


// -----------------------------------------------
//  If the number of seats to fill is less than
//  one, exit with an error.

        if ( global_number_of_seats_to_fill_for_case_type[ global_case_type ] < 1 )
        {
            log_out << "[Error: Number of seats to fill is less than one]" << std::endl ;
            std::cout << "Error: Number of seats to fill is less than one." << std::endl ;
            exit( EXIT_FAILURE ) ;
        }


// -----------------------------------------------
//  If this case begins a new cycle of case types,
//  initialize variables that are specific to the
//  case type, and randomly generate new ballot
//  rankings, and show the new ballot info in the
//  log file.

        if ( global_case_type == 1 )
        {
            global_candidate_who_won_votefair_seat_number[ 1 ] = 0 ;
            global_candidate_who_won_votefair_seat_number[ 2 ] = 0 ;
            generate_ballots( ) ;
            log_out << std::endl << std::endl ;
            for ( pointer_to_list_voteinfo = 1 ; pointer_to_list_voteinfo <= global_pointer_to_list_voteinfo_output_ballot ; pointer_to_list_voteinfo ++ )
            {
                log_out << global_list_voteinfo_output_ballot[ pointer_to_list_voteinfo ] << " " ;
                if ( pointer_to_list_voteinfo > 1 )
                {
                    if ( global_list_voteinfo_output_ballot[ pointer_to_list_voteinfo - 1 ] == global_voteinfo_code_for_question_number )
                    {
                        log_out << " " ;
                    }
                }
                if ( global_list_voteinfo_output_ballot[ pointer_to_list_voteinfo ] == global_voteinfo_code_for_end_of_ballot )
                {
                    log_out << std::endl ;
                }
            }
            log_out << std::endl << std::endl ;
        }


// -----------------------------------------------
//  Log the case number and case type.

        log_out << std::endl << "[case type " << global_case_id << "][" ;
        log_case_type_info( ) ;
        log_out << "]" << std::endl ;


// -----------------------------------------------
//  Initialize the pointers that point to the last
//  position in the voteinfo output lists.

        global_pointer_to_list_voteinfo_output_begin = 0 ;
        global_pointer_to_list_voteinfo_output_end = 0 ;


// -----------------------------------------------
//  Generate the voteinfo numbers that appear at
//  the beginning of the voteinfo output file.

        if ( global_yes_or_no_show_details_in_log_file == global_no )
        {
            global_pointer_to_list_voteinfo_output_begin ++ ;
            global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = global_voteinfo_code_for_request_logging_off ;
        }

        global_pointer_to_list_voteinfo_output_begin ++ ;
        global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = global_voteinfo_code_for_start_of_all_cases ;


// -----------------------------------------------
//  Specify the case number.

        global_pointer_to_list_voteinfo_output_begin ++ ;
        global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = global_voteinfo_code_for_case_number ;
        global_pointer_to_list_voteinfo_output_begin ++ ;
        global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = global_case_id ;


// -----------------------------------------------
//  Specify question number one, which must appear
//  after the case number.

        global_pointer_to_list_voteinfo_output_begin ++ ;
        global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = global_voteinfo_code_for_question_number ;
        global_pointer_to_list_voteinfo_output_begin ++ ;
        global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = 1 ;


// -----------------------------------------------
//  Specify the number of candidates.

        global_pointer_to_list_voteinfo_output_begin ++ ;
        global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = global_voteinfo_code_for_number_of_candidates ;
        global_pointer_to_list_voteinfo_output_begin ++ ;
        global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = global_full_candidate_count ;


// -----------------------------------------------
//  When VoteFair Ranking results are requested,
//  request VoteFair representation ranking
//  results.

        if ( global_case_type == global_case_type_votefair_representation )
        {
            global_pointer_to_list_voteinfo_output_begin ++ ;
            global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = global_voteinfo_code_for_request_votefair_representation_rank ;
        }


// -----------------------------------------------
//  Specify the number of seats to fill.  This
//  number switches to one when requesting
//  VoteFair popularity ranking results.

        global_pointer_to_list_voteinfo_output_begin ++ ;
        global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = global_voteinfo_code_for_number_of_equivalent_seats ;
        global_pointer_to_list_voteinfo_output_begin ++ ;
        global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = global_number_of_seats_to_fill_for_case_type[ global_case_type ] ;


// -----------------------------------------------
//  For some of the case types, request not
//  eliminating pairwise losing candidates, which
//  is done by requesting instant runoff voting.

        if ( global_yes_or_no_eliminate_pairwise_losing_candidate_for_case_type[ global_case_type ] == global_no )
        {
            global_pointer_to_list_voteinfo_output_begin ++ ;
            global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = global_voteinfo_code_for_request_instant_runoff_voting ;
        }


// -----------------------------------------------
//  For some of the case types, request the Droop
//  quota instead of the Hare quota.

        if ( global_yes_or_no_switch_to_droop_quota_for_case_type[ global_case_type ] == global_yes )
        {
            global_pointer_to_list_voteinfo_output_begin ++ ;
            global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = global_voteinfo_code_for_request_quota_droop_not_hare ;
        }


// -----------------------------------------------
//  For some of the case types, request ignoring
//  ballots on which a voter marks two or more
//  candidates at the same preference level.

        if ( global_yes_or_no_request_ignore_shared_rankings_for_case_type[ global_case_type ] == global_yes )
        {
            global_pointer_to_list_voteinfo_output_begin ++ ;
            global_list_voteinfo_output_begin[ global_pointer_to_list_voteinfo_output_begin ] = global_voteinfo_code_for_request_ignore_shared_rankings ;
        }


// -----------------------------------------------
//  Generate the codes at the end of the output file.

        global_pointer_to_list_voteinfo_output_end ++ ;
        global_list_voteinfo_output_end[ global_pointer_to_list_voteinfo_output_end ] = global_voteinfo_code_for_end_of_all_vote_info ;
        global_pointer_to_list_voteinfo_output_end ++ ;
        global_list_voteinfo_output_end[ global_pointer_to_list_voteinfo_output_end ] = global_voteinfo_code_for_end_of_all_cases ;


// -----------------------------------------------
//  Write the voteinfo codes to the output file
//  that will be supplied to the vote-counting
//  software.

        std::fstream outfile ;
        outfile.open ( "temp_generated_random_ballots.txt" , std::fstream::out ) ;
        for ( pointer_to_list_voteinfo = 1 ; pointer_to_list_voteinfo <= global_pointer_to_list_voteinfo_output_begin ; pointer_to_list_voteinfo ++ )
        {
            outfile << convert_integer_to_text( global_list_voteinfo_output_begin[ pointer_to_list_voteinfo ] ) << " " ;
        }
        outfile << std::endl << std::endl ;
        for ( pointer_to_list_voteinfo = 1 ; pointer_to_list_voteinfo <= global_pointer_to_list_voteinfo_output_ballot ; pointer_to_list_voteinfo ++ )
        {
            outfile << convert_integer_to_text( global_list_voteinfo_output_ballot[ pointer_to_list_voteinfo ] ) << " " ;
            if ( pointer_to_list_voteinfo > 1 )
            {
                if ( global_list_voteinfo_output_ballot[ pointer_to_list_voteinfo - 1 ] == global_voteinfo_code_for_question_number )
                {
                    outfile << " " ;
                }
                if ( global_list_voteinfo_output_ballot[ pointer_to_list_voteinfo ] == global_voteinfo_code_for_end_of_ballot )
                {
                    outfile << std::endl ;
                }
            }
        }
        outfile << std::endl ;
        for ( pointer_to_list_voteinfo = 1 ; pointer_to_list_voteinfo <= global_pointer_to_list_voteinfo_output_end ; pointer_to_list_voteinfo ++ )
        {
            outfile << convert_integer_to_text( global_list_voteinfo_output_end[ pointer_to_list_voteinfo ] ) << " " ;
        }
        outfile << std::endl ;
        outfile.close( ) ;


// -----------------------------------------------
//  Run the rcipe-stv-cpp program, or the
//  VoteFair Ranking program.
//  They read the file that was just written.
//
//  documentation:  https://cplusplus.com/reference/cstdlib/system/
//
//  If VoteFair popularity ranking is being done,
//  the first-seat winner from VoteFair
//  representation ranking will be used.

        integer_from_system_call = 0 ;
        if ( global_case_type == global_case_type_votefair_representation )
        {
            integer_from_system_call = system( ".\\votefair_ranking < temp_generated_random_ballots.txt > temp_calc_output.txt" ) ;
        } else if ( global_case_type == global_case_type_votefair_popularity )
        {
            do_nothing_here ++ ;
        } else
        {
            integer_from_system_call = system( ".\\rcipe_stv < temp_generated_random_ballots.txt > temp_calc_output.txt" ) ;
        }


// -----------------------------------------------
//  Join the log files to allow viewing details
//  of specific cases of interest.

        if ( global_case_type == global_case_type_votefair_representation )
        {
            integer_from_system_call = system( "type output_votefair_ranking_log.txt >> temp_joined_output_rcipe_stv_log.txt" ) ;
        } else if ( global_case_type == global_case_type_votefair_popularity )
        {
            do_nothing_here ++ ;
        } else
        {
            integer_from_system_call = system( "type output_rcipe_stv_log.txt >> temp_joined_output_rcipe_stv_log.txt" ) ;
        }


// -----------------------------------------------
//  Open the calculated-results file for reading,
//  then read the calculated results.

        if ( global_case_type != global_case_type_votefair_popularity )
        {
            calc_results.open ( "temp_calc_output.txt" , std::ios::in ) ;
            handle_calculated_results( ) ;
            calc_results.close ( ) ;
        }


// -----------------------------------------------
//  If VoteFair ranking encountered a tie, do not
//  check the other results because this case is
//  not a meaningful test, and restart the cycle
//  of tests with a new set of ballots.

        if ( ( global_case_type == global_case_type_votefair_representation ) && ( ( global_candidate_who_won_votefair_seat_number[ 1 ] < 1 ) || ( global_candidate_who_won_votefair_seat_number[ 2 ] < 1 ) ) )
        {
            log_out << "[VoteFair ranking encountered a tie, so ignore this case, now generating new ballots]" ;
            pointer_to_case_type = 0 ;
            continue ;
        }


// -----------------------------------------------
//  If one of the seats is not won by a majority
//  winner, log this mistake.

        if ( ( global_case_type != global_case_type_votefair_popularity ) && ( global_case_type != global_case_type_votefair_representation ) )
        {
            candidate_number = 0 ;
            for ( seat_number = 1 ; seat_number <= global_number_of_seats_to_fill_for_case_type[ global_case_type ] ; seat_number ++ )
            {
                if ( global_candidate_who_won_seat_number[ seat_number ] == global_majority_winner )
                {
                    candidate_number = global_majority_winner ;
                    break ;
                }
            }
            if ( candidate_number == 0 )
            {
                log_out << "[failure to elect majority winner!]" ;
            }
        }


// -----------------------------------------------
//  Count how many winners match the results of
//  VoteFair representation ranking.

        count_of_winners_matching = 0 ;
        if ( ( global_case_type != global_case_type_votefair_popularity ) && ( global_case_type != global_case_type_votefair_representation ) )
        {
            for ( votefair_seat_number = 1 ; votefair_seat_number <= 2 ; votefair_seat_number ++ )
            {
                for ( stv_seat_number = 1 ; stv_seat_number <= global_number_of_seats_to_fill_for_case_type[ global_case_type ] ; stv_seat_number ++ )
                {
                    if ( ( global_candidate_who_won_votefair_seat_number[ votefair_seat_number ] == global_candidate_who_won_seat_number[ stv_seat_number ] ) && ( global_candidate_who_won_votefair_seat_number[ votefair_seat_number ] ) )
                    {
                        count_of_winners_matching ++ ;
                        log_out << "[vf winner " << global_candidate_who_won_votefair_seat_number[ votefair_seat_number ] << ", stv winner " << global_candidate_who_won_seat_number[ stv_seat_number ] << "]" ;
                    }
                }
            }
            log_out << "[" << count_of_winners_matching << " winners match for this case]" ;
        }


// -----------------------------------------------
//  Calculate the satisfaction percent.

        calculate_satisfaction( ) ;


// -----------------------------------------------
//  Repeat the loop that handles each case.

    }


// -----------------------------------------------
//  Write the test conditions.

    log_out << std::endl << std::endl << std::endl ;

    log_out << "TEST CONDITIONS:" << std::endl ;
    log_out << global_full_candidate_count << " candidates" << std::endl ;
    log_out << global_total_ballot_count << " ballots" << std::endl ;
    log_out << global_count_ballot_generation_done << " ballot scenarios" << std::endl ;
    log_out << global_count_of_cases_ignored << " cases were ignored because not all the seats were filled" << std::endl << std::endl ;


// -----------------------------------------------
//  Write the list of case types and their ID
//  numbers in a way that can be used with Dashrep
//  code that analyzes the result data.

    for ( pointer_to_case_type = 1 ; pointer_to_case_type <= ( global_number_of_case_types_to_test + 2 ) ; pointer_to_case_type ++ )
    {
        global_case_type = global_list_of_case_types_to_test[ pointer_to_case_type ] ;
        log_out << "category-name-for-category-id-" << global_case_type << ":" << std::endl ;
        log_case_type_info( ) ;
        log_out << std::endl << "----" << std::endl ;
    }
    log_out << std::endl ;


// -----------------------------------------------
//  Write the case type ID numbers as a list that
//  is sorted by number of seats and other
//  characteristics.

    log_out << "list-of-category-ids:" << std::endl ;
    for ( yes_or_no_switch_to_droop_quota = global_no ; yes_or_no_switch_to_droop_quota <= global_yes ; yes_or_no_switch_to_droop_quota ++ )
    {
        for ( seats_to_fill = 1 ; seats_to_fill <= 5 ; seats_to_fill ++ )
        {
            for ( yes_or_no_eliminate_pairwise_losing_candidate = global_no ; yes_or_no_eliminate_pairwise_losing_candidate <= global_yes ; yes_or_no_eliminate_pairwise_losing_candidate ++ )
            {
                for ( yes_or_no_request_ignore_shared_rankings = global_no ; yes_or_no_request_ignore_shared_rankings <= global_yes ; yes_or_no_request_ignore_shared_rankings ++ )
                {
                    for ( global_case_type = 1 ; global_case_type <= ( global_number_of_case_types_to_test + 2 )  ; global_case_type ++ )
                    {
                        if ( ( global_yes_or_no_switch_to_droop_quota_for_case_type[ global_case_type ] == yes_or_no_switch_to_droop_quota ) && ( global_number_of_seats_to_fill_for_case_type[ global_case_type ] == seats_to_fill ) && ( global_yes_or_no_eliminate_pairwise_losing_candidate_for_case_type[ global_case_type ] == yes_or_no_eliminate_pairwise_losing_candidate ) && ( global_yes_or_no_request_ignore_shared_rankings_for_case_type[ global_case_type ] == yes_or_no_request_ignore_shared_rankings ) )
                        {
                            log_out << global_case_type << std::endl ;
                        }
                    }
                }
            }
        }
    }
    log_out << "----" << std::endl << std::endl ;


// -----------------------------------------------
//  Write the ballot repeat counts.

    for ( ballot_group_number = 1 ; ballot_group_number <= global_maximum_ballot_group ; ballot_group_number ++ )
    {
        log_out << "ballot group " << ballot_group_number << " has repeat count of " << global_repeat_count_for_ballot_group[ ballot_group_number ] << std::endl ;
    }
    log_out << std::endl ;


// -----------------------------------------------
//  Write the count of how many cases were
//  successfully calculated for each case type.

    for ( pointer_to_case_type = 1 ; pointer_to_case_type <= ( global_number_of_case_types_to_test + 2 ) ; pointer_to_case_type ++ )
    {
        global_case_type = global_list_of_case_types_to_test[ pointer_to_case_type ] ;
        log_out << "case type " << global_case_type << " calculated " << global_count_of_successful_cases_for_case_type[ global_case_type ] << " proportional satisfaction rates" << std::endl ;
    }
    log_out << std::endl ;


// -----------------------------------------------
//  Write the name of the file that contains the
//  results in a format that can be imported into
//  a spreadsheet or other software that
//  calculates the standard deviations and ranges.

    log_out << std::endl << "The results are in the spreadsheet-compatible file named " << "output_spreadsheet_data_satisfaction_rates.txt" << std::endl ;


// -----------------------------------------------
//  End of function do_all_tests

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//
//  Execution starts here.

int main( ) {

    int candidate_number ;
    int sequence_position ;


// -----------------------------------------------
//  Open the output log file.

    log_out.open ( "temp_log_from_measure_rcipe_stv_satisfaction_rate.txt" , std::ios::out ) ;


// -----------------------------------------------
//  Open the output spreadsheet file.

    spreadsheet_out.open ( "output_spreadsheet_data_satisfaction_rates.txt" , std::ios::out ) ;


// -----------------------------------------------
//  Initialization.

    do_initialization( ) ;


// -----------------------------------------------
//  Do the requested tests.

    do_all_tests( ) ;


// -----------------------------------------------
//  If desired, write to a file the counts that
//  can be looked at to verify that the preference
//  info is really random.
//  Use either "1 == 1" or "1 == 0" to generate or
//  omit this output file.

    if ( 1 == 0 )
//    if ( 1 == 1 )
    {
        std::fstream verifyfile;
        verifyfile.open ( "temp_verify_randomness.txt" , std::fstream::app ) ;
        for ( candidate_number = 1 ; candidate_number <= global_full_candidate_count ; candidate_number ++ )
        {
            for ( sequence_position = 1 ; sequence_position <= global_full_candidate_count ; sequence_position ++ )
            {
                verifyfile << "candidate " << candidate_number << " seq_pos " << sequence_position << " usage " << global_usage_count_for_candidate_and_sequence_position[ candidate_number ][ sequence_position ] << std::endl ;
            }
        }
        verifyfile.close( ) ;
    }


// -----------------------------------------------
//  End of "main" code.

}


// -----------------------------------------------
// -----------------------------------------------
//
//  End of all code.
//
// -----------------------------------------------
// -----------------------------------------------


// -----------------------------------------------
//
//  AUTHOR
//
//  Richard Fobes, www.VoteFair.org
//
//
// -----------------------------------------------
//
//  BUGS
//
//  Please report any bugs or feature requests on GitHub,
//  at the CPSolver account, in the VoteFairRanking project
//  area.  Thank you!
//
//
// -----------------------------------------------
//
//  SUPPORT
//
//  You can find documentation for this code on GitHub,
//  in the CPSolver account, in the VoteFair Ranking project
//  areas.
//
//  You can find details about VoteFair Ranking at: www.VoteFair.org
//
//
// -----------------------------------------------
//
//  COPYRIGHT & LICENSE
//
//  (c) Copyright 2022 by Richard Fobes at www.VoteFair.org.
//  You can redistribute and/or modify this VoteFairRanking software
//  under the MIT software license terms as explained in the
//  LICENSE file.)
//
//
// -----------------------------------------------
//
//  End of measure_rcipe_stv_satisfaction_rate.cpp
