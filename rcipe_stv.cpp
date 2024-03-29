// -----------------------------------------------
// -----------------------------------------------
//
//  rcipe_stv.cpp
//
//  This software utility calculates election
//  results for the following methods:
//
//  * STV (the Single Transferable Vote)
//
//  * RCIPE STV
//
//  * IRV (Instant Runoff Voting)
//
//  * RCIPE
//
//  RCIPE is the abbreviation for "Ranked Choice
//  Including Pairwise Elimination."
//
//  IMPORTANT:
//  For all these calculations, ballots on which a
//  voter marks more than one candidate at the
//  same preference level are counted instead of
//  being discarded.
//
//  When shared preference levels are encountered,
//  the ballots are transfered in "whole" numbers,
//  not by splitting a ballot into fractional or
//  decimal portions.  For example, during a
//  counting cycle, if there are two ballots that
//  rank candidates numbered 1 and 2 at the same
//  highest ranking level, one of the ballots will
//  transfer to candidate 1 and the other ballot
//  will transfer to candidate 2.
//
//  Unlike some simplistic versions of STV
//  software, during each counting cycle either a
//  candidate wins an available seat, or an
//  unpopular candidate is eliminated, but not
//  both in the same counting cycle.  Also, only
//  one candidate can get elected in each counting
//  cycle because ballots are transfered after
//  each seat is filled.
//
//  The RCIPE and RCIPE STV methods are described
//  at:
//  https://electowiki.org/wiki/Ranked_Choice_Including_Pairwise_Elimination
//
//  To specify IRV or RCIPE calculations, specify
//  that just a single seat is available to be
//  filled.  To request STV or RCIPE STV
//  calculations, specify the number of seats to
//  be filled, with the typical number of seats
//  being 2, 3, 4, or 5.  Use voteinfo code -67
//  followed by the number of seats to be filled.
//  There is no default choice for this seat
//  count.
//
//  To specify IRV or STV instead of RCIPE or
//  RCIPE STV, request that pairwise losing
//  candidates not be eliminated when they occur.
//  Otherwise, the default is to eliminate
//  pairwise losing candidates when they occur.
//  A pairwise losing candidate is a candidate who
//  would lose every one-on-one contest against
//  every other remaining candidate.  This extra
//  elimination prevents a ballot from getting
//  "stuck" on a candidate who a majority of
//  voters dislike, which causes that ballot's
//  lower ranking marks to be ignored while other
//  ballots control which of the more-popular
//  candidates win and lose.  The default choice
//  is to eliminate pairwise losing candidates
//  when they occur.  Use voteinfo code -50 to
//  request IRV or STV.
//
//  When calculating STV or RCIPE STV results,
//  either the Hare quota or the Droop quota can
//  be selected.  The default choice is the Hare
//  quota.  Use voteinfo code -68 to request the
//  Droop quota.
//
//  Ballot counting is done in ways that avoid
//  getting different results if the supplied
//  ballot sequence is changed.  Electing only one
//  candidate per counting cycle is one such way
//  of avoiding inconsistent results.
//
//  Additional details appear below within the
//  code comments.
//
// 
// -----------------------------------------------
//
//  COPYRIGHT & LICENSE
//
//  (c) Copyright 2022 by Richard Fobes at
//  www.VoteFair.org.  You can redistribute and/or
//  modify this rcipe_stv software under the MIT
//  software license terms that appear below.
//  (Also a copy of the license is included in the
//  "license" file.)
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
//  Version 0.10 - In April 2022 Richard Fobes
//  derived this code from the
//  VoteFair_Ranking.cpp code that calculates
//  VoteFair Ranking, and single-winner RCIPE,
//  and other single-winner vote-counting methods.
//
//  Version 0.15 - In May 2022 Richard Fobes
//  did testing and debugging, and improved
//  comments, and updated the version count to:
//  0.15  Yet more rigorous testing is needed
//  before relying on this code for elections.
//
//  Version 0.20 - In June 2022 removed a
//  potential bug, after more successful
//  testing.
//
// -----------------------------------------------
//
//  USAGE
//
//  The following sample code compiles and
//  executes this software under a typical
//  Windows environment with the g++ compiler and
//  the mingw32 library already installed.
//
//      path=C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin\
//
//      g++ rcipe_stv.cpp -o rcipe_stv
//
//      .\rcipe_stv < input_rcipe_stv_case_123.txt > output_rcipe_stv_case_123.txt
//
//  This usage assumes that file
//  input_rcipe_stv_case_123.txt contains
//  appropriately formatted election/survey/poll
//  data for case numbered 123, and it writes
//  the coded results to file
//  output_rcipe_stv_case_123.txt.
//
//  Typically the input file is generated by
//  other software, and typically the output file
//  is used as input to other software.  An
//  example of such software is the
//  "VoteFair-polls" code on GitHub (in the
//  CPSolver repository).
//
//  The input file must contain integer codes that
//  have the meanings specified in the constants
//  that begin with "global_voteinfo_code_for_...".
//
//  The mathematical algorithms of RCIPE and
//  RCIPE STV are in the public domain.
//
//
// -----------------------------------------------
//
//  SAMPLE INPUT FILE
//
//  The following text (excluding the "//" text)
//  represents the data in the example in the
//  Wikipedia article titled "Comparison of the
//  Hare and Droop quotas".
//
//  -7
//  -3 1000001
//  -4 1
//  -6 6
//  -67 5
//  -50
//  -9 -4 1 -11 31 1 2 3 -10
//  -9 -4 1 -11 30 3 1 2 -10
//  -9 -4 1 -11 2 2 1 3 -10
//  -9 -4 1 -11 20 4 5 6 -10
//  -9 -4 1 -11 20 5 4 6 -10
//  -9 -4 1 -11 17 6 4 5 -10
//  -8
//
//
// -----------------------------------------------
//
//  LICENSE
//
//  This rcipe_stv software is licensed
//  under the following MIT License terms.
//
//  MIT License for rcipe_stv.cpp
//
//  Copyright (c) 2022 by Richard Fobes at
//  ww.VoteFair.org
//
//  Permission is hereby granted, free of charge,
//  to any person obtaining a copy of this software
//  and associated documentation files (the
//  "Software"), to deal in the Software without
//  restriction, including without limitation the
//  rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of
//  the Software, and to permit persons to whom
//  the Software is furnished to do so, subject
//  to the following conditions:
//
//  The above copyright notice and this
//  permission notice shall be included in all
//  copies or substantial portions of the
//  Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT
//  WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
//  ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
//  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//
//
// -----------------------------------------------
//
//      IMPORTANT NOTES TO CODERS!
//
//  Please report any bugs or feature requests on
//  GitHub, at the CPSolver account, in the
//  VoteFair-ranking-cpp project area.  Thank you!
//
//  If you offer improvements to this code,
//  please follow the conventions used here, and
//  please keep this code easy to convert into
//  other programming languages.
//
//  Please resist the temptation to add the
//  handling of candidate names or any other text
//  that appears on ballots.  The absense of that
//  text makes it obvious that this code is
//  completely unbiased -- because candidate
//  names, party names, and election names are
//  not available to this software.  Instead an
//  election and candidates are identified by
//  integer numbers (1, 2, 3, etc.) and only the
//  software that assigned these numbers can
//  correlate them with any names or identities.
//
//
// -----------------------------------------------


// -----------------------------------------------
//  Begin code.
// -----------------------------------------------


// -----------------------------------------------
//  Specify libraries needed.

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>


// -----------------------------------------------
//  Declare global variables, lists, etc.


//  Declare the case number as a variable.
//  It is received from the input file and copied
//  to the output file to allow verifying a match
//  between the ballots and the results.

int global_case_number ;


//  Declare global variables that are
//  controlled by voteinfo codes.

int global_number_of_seats_to_fill ;
int global_true_or_false_request_no_pairwise_loser_elimination ;
int global_true_or_false_request_quota_droop ;
int global_true_or_false_request_ignore_shared_rankings ;


//  Declare global variables.

int global_input_line_number ;
int global_current_voteinfo_number ;
int global_next_voteinfo_number ;
int global_previous_voteinfo_number ;
int global_pointer_to_voteinfo_number ;
int global_pointer_to_end_of_voteinfo_numbers ;
int global_within_ballots ;
int global_question_number ;
int global_candidate_number ;
int global_candidate_just_elected ;
int global_number_of_candidates ;
int global_number_of_remaining_candidates ;
int global_count_of_candidates_marked ;
int global_count_of_top_ranked_remaining_candidates ;
int global_number_of_seats_still_available ;
int global_ballot_info_repeat_count ;
int global_current_total_vote_count ;
int global_supporting_votes_for_elected_candidate ;
int global_supporting_vote_count_that_exceeds_quota ;
int global_need_to_initialize_group_ballot_count ;
int global_quota_count ;
int global_counting_cycle_number ;
int global_ballot_group_pointer ;
int global_total_count_of_ballot_groups ;
int global_count_of_unique_pattern_numbers ;
int global_pair_counter_maximum ;
int global_pointer_to_output_results ;
int global_length_of_result_info_list ;
int global_logging_info ;


//  Specify the true and false values.
//  These are used instead of language-specific codes
//  because different programming languages use
//  different conventions for their true/false logic.

const int global_true = 1 ;
const int global_false = 0 ;


//  For speed reasons, arrays are declared here,
//  not within a function, even if a single
//  function uses them.

//  Position one -- [ 1 ] -- is used as the
//  starting position for these arrays, so the
//  array lengths have to be longer than the
//  number of items to be stored.


//  These arrays have one item per candidate.

const int global_maximum_candidate_number = 100 ;

int global_true_or_false_winner_candidate[ 101 ] ;
int global_true_or_false_eliminated_candidate[ 101 ] ;
int global_true_or_false_available_candidate[ 101 ] ;
int global_true_or_false_is_top_ranked_candidate[ 101 ] ;
int global_true_or_false_pairwise_consider_candidate[ 101 ] ;
int global_ballot_preference_for_candidate[ 101 ] ;
int global_vote_transfer_count_for_candidate[ 101 ] ;
int global_win_count_for_candidate[ 101 ] ;
int global_loss_count_for_candidate[ 101 ] ;
int global_tally_uses_of_candidate_number[ 101 ] ;
int global_list_of_top_ranked_candidates[ 101 ] ;
int global_list_of_candidates_with_highest_vote_transfer_count[ 101 ] ;
int global_list_of_candidates_with_lowest_vote_transfer_count[ 101 ] ;
int global_list_of_candidates_tied[ 101 ] ;


//  Declare the input-related list.
//  Allow room for extra codes at the end.

const int global_maximum_vote_info_list_length = 200000 ;

int global_vote_info_list[ 200005 ] ;


//  Declare the output-related list.
//  Allow room for extra codes at the end.

const int global_maximum_output_results_length = 2000 ;

int global_output_results[ 2005 ] ;


//  Declare pairwise lists.

const int global_maximum_candidate_pairs = 20000 ;

int global_first_candidate_number_in_pair[ 2001 ] ;
int global_second_candidate_number_in_pair[ 2001 ] ;
int global_tally_first_over_second_in_pair[ 2001 ] ;
int global_tally_second_over_first_in_pair[ 2001 ] ;
int global_tally_first_equal_second_in_pair[ 2001 ] ;


//  Declare the lists that group identical ballots
//  together for faster processing.

const int global_maximum_number_of_ballot_groups = 20000 ;

int global_ballot_count_remaining_for_ballot_group[ 20001 ] ;
int global_top_ranked_candidate_for_ballot_group[ 20001 ] ;


//  Declare the lists that combine the counting of
//  ballots that have the same equivalent top-ranked
//  candidates (during that counting cycle).

const int global_maximum_number_of_pattern_numbers = 10000 ;

int global_pattern_number_for_pattern_number_pointer[ 10001 ] ;
int global_ballot_count_for_pattern_number_pointer[ 10001 ] ;
int global_top_candidate_count_for_pattern_number_pointer[ 10001 ] ;


//  Note:  Do NOT change these numbers!  They
//  match codes used in the
//  VoteFair_Ranking.cpp application.
//  Some of these codes supply ballot information
//  and make requests for various options.  Some
//  of these codes supply calculated results.
//  Some codes can be used for both input and
//  output.

const int global_voteinfo_code_for_start_of_all_cases = -1 ;
const int global_voteinfo_code_for_end_of_all_cases = -2 ;
const int global_voteinfo_code_for_case_number = -3 ;
const int global_voteinfo_code_for_question_number = -4 ;
const int global_voteinfo_code_for_total_ballot_count = -5 ;

//  In other software the following constant is named global_voteinfo_code_for_number_of_choices
const int global_voteinfo_code_for_number_of_candidates = -6 ;

const int global_voteinfo_code_for_start_of_all_vote_info = -7 ;
const int global_voteinfo_code_for_end_of_all_vote_info = -8 ;
const int global_voteinfo_code_for_start_of_ballot = -9 ;
const int global_voteinfo_code_for_end_of_ballot = -10 ;

//  The following ballot count indicates how many ballots have the same ranking.
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
const int global_voteinfo_code_for_start_of_votefair_party_ranking_sequence_results = -23 ;
const int global_voteinfo_code_for_end_of_votefair_party_ranking_sequence_results = -24 ;
const int global_voteinfo_code_for_start_of_votefair_party_ranking_levels_results = -25 ;
const int global_voteinfo_code_for_end_of_votefair_party_ranking_levels_results = -26 ;
const int global_voteinfo_code_for_ranking_level = -27 ;
const int global_voteinfo_code_for_next_ranking_level = -28 ;
const int global_voteinfo_code_for_early_end_of_ranking = -29 ;
const int global_voteinfo_code_for_start_of_tally_table_results = -30 ;
const int global_voteinfo_code_for_end_of_tally_table_results = -31 ;
const int global_voteinfo_code_for_first_choice = -32 ;
const int global_voteinfo_code_for_second_choice = -33 ;
const int global_voteinfo_code_for_tally_first_over_second = -34 ;
const int global_voteinfo_code_for_tally_second_over_first = -35 ;
const int global_voteinfo_code_for_start_of_plurality_results = -36 ;
const int global_voteinfo_code_for_end_of_plurality_results = -37 ;
const int global_voteinfo_code_for_plurality_count = -38 ;
const int global_voteinfo_code_for_skip_case = -39 ;
const int global_voteinfo_code_for_skip_question = -40 ;
const int global_voteinfo_code_for_request_votefair_representation_rank = -41 ;
const int global_voteinfo_code_for_request_no_votefair_representation_rank = -42 ;
const int global_voteinfo_code_for_request_votefair_party_rank = -43 ;
const int global_voteinfo_code_for_request_no_votefair_party_rank = -44 ;
const int global_voteinfo_code_for_request_only_plurality_results = -45 ;
const int global_voteinfo_code_for_request_pairwise_counts = -46 ;
const int global_voteinfo_code_for_request_no_pairwise_counts = -47 ;
const int global_voteinfo_code_for_number_of_representation_levels_to_compute = -48 ;
const int global_voteinfo_code_for_request_text_output = -49 ;
const int global_voteinfo_code_for_request_instant_runoff_voting = -50 ;
const int global_voteinfo_code_for_request_instant_pairwise_elimination = -51 ;
const int global_voteinfo_code_for_request_rcipe_voting = -52 ;
const int global_voteinfo_code_for_winner_instant_runoff_voting = -53 ;
const int global_voteinfo_code_for_winner_instant_pairwise_elimination = -54 ;
const int global_voteinfo_code_for_winner_rcipe_voting = -55 ;
const int global_voteinfo_code_for_request_star_voting = -56 ;
const int global_voteinfo_code_for_winner_star_voting = -57 ;
const int global_voteinfo_code_for_request_pairwise_loser_elimination = -58 ;
const int global_voteinfo_code_for_winner_pairwise_loser_elimination = -59 ;
const int global_voteinfo_code_for_winner_irv_bottom_two_runoff = -60 ;
const int global_voteinfo_code_for_winner_borda_count = -61 ;
const int global_voteinfo_code_for_flag_as_interesting = -62 ;
const int global_voteinfo_code_for_winner_approval_voting = -63 ;
const int global_voteinfo_code_for_winner_condorcet = -64 ;
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

const int global_voteinfo_code_for_invalid_input_word = -200 ;


//  Specify an extra output file that contains a log
//  of actions for the purpose of monitoring or
//  debugging intermediate calculations.

std::ofstream log_out ;


//  Declare message strings.

std::string global_possible_error_message ;
std::string global_text_quota_type ;
std::string global_text_quota_type_hare ;
std::string global_text_quota_type_droop ;
std::string global_text_quota_type_majority ;


// -----------------------------------------------
// -----------------------------------------------
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
//  convert_text_to_integer
//
//  To read why this function is here, see the comment
//  above for function: convert_integer_to_text

int convert_text_to_integer( char * supplied_text )
{
    int equivalent_integer ;
    try
    {
        equivalent_integer = atoi( supplied_text ) ;
    }
    catch( ... )
    {
        equivalent_integer = -999 ;
    }
    return equivalent_integer ;
}


// -----------------------------------------------
// -----------------------------------------------
//    do_main_initialization
//
//  Does initialization at the very beginning.

void do_main_initialization( )
{

    int pointer ;
    int candidate_number ;


// -----------------------------------------------
//  Reset logging flag.

    global_logging_info = global_true ;


// -----------------------------------------------
//  Open the output file that logs details.

    log_out.open ( "output_rcipe_stv_log.txt" , std::ios::out ) ;


// -----------------------------------------------
//  Initialize zero values.

    global_current_voteinfo_number = 0 ;
    global_previous_voteinfo_number = 0 ;
    global_next_voteinfo_number = 0 ;
    global_ballot_info_repeat_count = 0 ;
    global_current_total_vote_count = 0 ;
    global_pointer_to_voteinfo_number = 0 ;
    global_pointer_to_end_of_voteinfo_numbers = 0 ;
    global_pointer_to_output_results = 0 ;
    global_case_number = 0 ;
    global_length_of_result_info_list = 0 ;
    global_number_of_candidates = 0 ;
    global_ballot_group_pointer = 0 ;
    global_input_line_number = 0 ;


// -----------------------------------------------
//  Initialize flags.

    global_true_or_false_request_quota_droop = global_false ;
    global_true_or_false_request_no_pairwise_loser_elimination = global_false ;
    global_true_or_false_request_ignore_shared_rankings = global_false ;


// -----------------------------------------------
//  Initialize text values.

    global_possible_error_message = "" ;
    global_text_quota_type_hare = "Hare" ;
    global_text_quota_type_droop = "Droop" ;
    global_text_quota_type_majority = "majority" ;
    global_text_quota_type = global_text_quota_type_hare ;


// -----------------------------------------------
//  Initialize lists to zeros.

    for ( pointer = 0 ; pointer <= global_maximum_vote_info_list_length ; pointer ++ )
    {
        global_vote_info_list[ pointer ] = 0 ;
    }
    for ( pointer = 0 ; pointer <= global_maximum_output_results_length ; pointer ++ )
    {
        global_output_results[ pointer ] = 0 ;
    }


// -----------------------------------------------
//  Initialize the candidate-specific flags.

    for ( candidate_number = 0 ; candidate_number <= global_maximum_candidate_number ; candidate_number ++ )
    {
        global_true_or_false_available_candidate[ candidate_number ] = global_true ;
        global_true_or_false_winner_candidate[ candidate_number ] = global_false ;
        global_true_or_false_eliminated_candidate[ candidate_number ] = global_false ;
    }


// -----------------------------------------------
//  End of function do_main_initialization.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//       save_ballot_info_number
//
//  Puts the next voteinfo number into the list
//  that stores the ballot-specific information.

void save_ballot_info_number( int voteinfo_number )
{


// -----------------------------------------------
//  Increment the pointer to the voteinfo numbers
//  being stored in the list.
//  Note that list position zero ([0]) is not used!

    global_pointer_to_voteinfo_number ++ ;


// -----------------------------------------------
//  If the list has become too long, indicate that
//  fatal error.

    if ( global_pointer_to_voteinfo_number > global_maximum_vote_info_list_length )
    {
        if ( global_logging_info == global_true ) { log_out << "[error, too many vote-info numbers supplied, the available storage space must be increased]" ; } ;
        std::cout << "Error: Too many vote-info numbers supplied, the available storage space must be increased." ;
        exit( EXIT_FAILURE ) ;
    }


// -----------------------------------------------
//  Store the supplied ballot-specific voteinfo
//  number.

    global_vote_info_list[ global_pointer_to_voteinfo_number ] = voteinfo_number ;
//    if ( global_logging_info == global_true ) { log_out << "[at " << global_pointer_to_voteinfo_number << " vicode " << voteinfo_number << "]" ; } ;


// -----------------------------------------------
//  Insert an end-of-info code number at the next
//  position, in case this is the last voteinfo
//  number put into the list.

    global_vote_info_list[ global_pointer_to_voteinfo_number + 1 ] = global_voteinfo_code_for_end_of_all_vote_info ;


// -----------------------------------------------
//  End of function save_ballot_info_number.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//       put_next_result_info_number
//
//  Puts the next result-info number into the array
//  that stores the result information.

void put_next_result_info_number( int current_result_info_number )
{


// -----------------------------------------------
//  If the list has become too long, insert the
//  code that indicates the end of the results,
//  and then indicate an error.

    if ( global_pointer_to_output_results >= global_maximum_output_results_length )
    {
        global_output_results[ global_pointer_to_output_results ] = global_voteinfo_code_for_end_of_all_cases ;
        if ( global_logging_info == global_true ) { log_out << "[error, not enough room for all results (size limit is " << convert_integer_to_text( global_maximum_output_results_length ) << "]" ; } ;
        global_possible_error_message = "Error: Not enough room for all results (size limit is " + convert_integer_to_text( global_maximum_output_results_length ) + ")." ;
        return ;
    }


// -----------------------------------------------
//  Put the next result-info number into the list.

    global_output_results[ global_pointer_to_output_results ] = current_result_info_number ;


// -----------------------------------------------
//  Increment the list pointer, and increment the
//  length of the list.

    global_pointer_to_output_results ++ ;
    global_length_of_result_info_list = global_pointer_to_output_results ;


// -----------------------------------------------
//  End of function put_next_result_info_number.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//       handle_one_voteinfo_number
//
//  Handles each voteinfo number, one at a time.
//  Some voteinfo numbers specify an integer
//  number, such as the case number, the number of
//  candidates, and the number of equivalent seats
//  to fill.  Those numbers are saved in special
//  variables.  The remaining numbers, which
//  indicate ballot preferences, and repeat counts
//  -- for identical ballots -- are stored in a
//  list that will be accessed repeatedly during
//  vote counting.  Also this function checks for
//  errors, and reports any that occur.

void handle_one_voteinfo_number( )
{

    int candidate_number ;
    int true_or_false_have_handled_one_unranked_candidate ;


// -----------------------------------------------
//  If the code is a ballot repeat count, or is
//  the end of the ballot data, and if not all the
//  candidate numbers have been encountered for
//  this ballot, rank these not-encountered
//  candidates at the level below the last
//  candidate encountered.  Do not return yet
//  because the ballot repeat count for the new
//  ballot group needs to be saved.

    if ( ( global_total_count_of_ballot_groups >= 1 ) && ( ( global_current_voteinfo_number == global_voteinfo_code_for_ballot_count ) || ( global_current_voteinfo_number == global_voteinfo_code_for_end_of_all_cases ) || ( global_current_voteinfo_number == global_voteinfo_code_for_end_of_all_vote_info ) ) )
    {
        true_or_false_have_handled_one_unranked_candidate = global_false ;
        for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
        {
            if ( global_tally_uses_of_candidate_number[ candidate_number ] < 1 )
            {
                if ( true_or_false_have_handled_one_unranked_candidate == global_true )
                {
                    save_ballot_info_number( global_voteinfo_code_for_tie ) ;
                }
                save_ballot_info_number( candidate_number ) ;
                true_or_false_have_handled_one_unranked_candidate = global_true ;
//                if ( global_logging_info == global_true ) { log_out << "[unmarked candidate " << candidate_number << "]" ; } ;
            }
        }
    }


// -----------------------------------------------
//  If the current code indicates a ballot repeat
//  count, clear the flags that track which
//  candidate numbers are encountered in this
//  ballot group.  Do not return yet.

    if ( global_current_voteinfo_number == global_voteinfo_code_for_ballot_count )
    {
        for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
        {
            global_tally_uses_of_candidate_number[ candidate_number ] = 0 ;
        }
    }


// -----------------------------------------------
//  Handle the end of a case or the end of the
//  ballot info, check for any errors, and return.

    if ( ( global_current_voteinfo_number == global_voteinfo_code_for_end_of_all_cases ) || ( global_current_voteinfo_number == global_voteinfo_code_for_end_of_all_vote_info ) )
    {
        if ( global_case_number > 0 )
        {
            if ( global_ballot_info_repeat_count == 0 )
            {
                if ( global_logging_info == global_true ) { log_out << "[error, no ballots found]" ; } ;
                global_possible_error_message = "Error: No ballots found." ;
                return ;
            }
            if ( global_number_of_seats_to_fill < 1 )
            {
                if ( global_logging_info == global_true ) { log_out << "[error, seats to fill is less than one]" ; } ;
                global_possible_error_message = "Error: Seats to fill is less than one." ;
                return ;
            }
        } else if ( global_case_number < 1 )
        {
            if ( global_logging_info == global_true ) { log_out << "[error, case number not specified]" ; } ;
            global_possible_error_message = "Error: Case number was not specified." ;
            return ;
        }
        return ;
    }


// -----------------------------------------------
//  Get the ballot repeat count, increment the
//  counter for the number of ballot groups, and
//  save the ballot repeat count in the
//  ballot-info list, then return.

    if ( global_previous_voteinfo_number == global_voteinfo_code_for_ballot_count )
    {
        global_ballot_info_repeat_count = global_current_voteinfo_number ;
        save_ballot_info_number( global_voteinfo_code_for_ballot_count ) ;
        save_ballot_info_number( global_ballot_info_repeat_count ) ;
        global_count_of_candidates_marked = 0 ;
        global_total_count_of_ballot_groups ++ ;
        if ( global_logging_info == global_true ) { log_out << "[bc " << global_ballot_info_repeat_count << "]" ; } ;
        if ( global_ballot_info_repeat_count < 1 )
        {
            if ( global_logging_info == global_true ) { log_out << "[error, ballot count number is less than one (" << global_ballot_info_repeat_count << ")]" ; } ;
            global_possible_error_message = "Error: Ballot count number is less than one (" + convert_integer_to_text( global_ballot_info_repeat_count ) + ")." ;
        } else if ( global_total_count_of_ballot_groups >= global_maximum_number_of_ballot_groups )
        {
            if ( global_logging_info == global_true ) { log_out << "[error, number of ballot groups (" << global_total_count_of_ballot_groups << ") exceeds available storage space]" ; } ;
            global_possible_error_message = "Error: Number of ballot groups (" + convert_integer_to_text( global_total_count_of_ballot_groups ) + ") exceeds the available storage space." ;
        }
        return ;
    }


// -----------------------------------------------
//  Handle the code for a tie.  Save it in the
//  vote-info list, then return.

    if ( global_current_voteinfo_number == global_voteinfo_code_for_tie )
    {
        save_ballot_info_number( global_voteinfo_code_for_tie ) ;
        if ( global_logging_info == global_true ) { log_out << "[+]" ; } ;
        if ( global_count_of_candidates_marked < 1 )
        {
            if ( global_logging_info == global_true ) { log_out << "[error, invalid nesting of tied preference vote-info number, at input line number " << global_input_line_number << "]" ; } ;
            global_possible_error_message = "Error: Invalid nesting of tied preference vote-info number, at input line number " + convert_integer_to_text( global_input_line_number ) + "." ;
            return ;
        }
        global_count_of_candidates_marked = 0 ;
        return ;
    }


// -----------------------------------------------
//  Get the case number, then return.

    if ( global_previous_voteinfo_number == global_voteinfo_code_for_case_number )
    {
        if ( global_case_number != 0 )
        {
            if ( global_logging_info == global_true ) { log_out << "[error, second case number encountered, which is not allowed]" ; } ;
            global_possible_error_message = "Error: Second case number encountered, which is not valid." ;
            return ;
        }
        global_case_number = global_current_voteinfo_number ;
        global_within_ballots = global_false ;
        if ( global_logging_info == global_true ) { log_out << "[case " << global_case_number << "]" ; } ;
        if ( global_case_number < 1 )
        {
            if ( global_logging_info == global_true ) { log_out << "[error, case number is less than one, which is not valid]" ; } ;
            global_possible_error_message = "Error: Case number is less than one, which is not valid." ;
        }
        return ;
    }


// -----------------------------------------------
//  Get the question number, which must be one,
//  then return.
//  Reminder: This software handles only one
//  question, but the voteinfo codes support
//  multiple questions.

    if ( global_previous_voteinfo_number == global_voteinfo_code_for_question_number )
    {
        global_question_number = global_current_voteinfo_number ;
        if ( global_question_number != 1 )
        {
            if ( global_logging_info == global_true ) { log_out << "[error, question number is not one (" << global_question_number << ")]" ; } ;
            global_possible_error_message = "Error: Encountered question number that is not one (" + convert_integer_to_text( global_question_number ) + ")." ;
        }
        return ;
    }


// -----------------------------------------------
//  Get the count for the number of candidates,
//  then return.

    if ( global_previous_voteinfo_number == global_voteinfo_code_for_number_of_candidates )
    {
        global_number_of_candidates = global_current_voteinfo_number ;
        if ( global_logging_info == global_true ) { log_out << "[candidate count " << global_number_of_candidates << "]" ; } ;
        if ( global_number_of_candidates == 1 )
        {
            if ( global_logging_info == global_true ) { log_out << "[error, only one candidate]" ; } ;
            global_possible_error_message = "Error: Only one candidate." ;
            return ;
        }
        if ( global_number_of_candidates < 1 )
        {
            if ( global_logging_info == global_true ) { log_out << "[error, no candidates specified]" ; } ;
            global_possible_error_message = "Error: No candidates specified." ;
            return ;
        }
        if ( global_number_of_candidates > global_maximum_candidate_number )
        {
            if ( global_logging_info == global_true ) { log_out << "[error, number of candidates, " << global_number_of_candidates << ", exceeds maximum]" ; } ;
            global_possible_error_message = "Error: Number of candidates, " + convert_integer_to_text( global_number_of_candidates ) + ", exceeds maximum." ;
            return ;
        }
        return ;
    }


// -----------------------------------------------
//  Get the number of equivalent seats to be
//  filled, write this number to the results info,
//  then return.

    if ( global_previous_voteinfo_number == global_voteinfo_code_for_number_of_equivalent_seats )
    {
        global_number_of_seats_to_fill = global_current_voteinfo_number ;
        put_next_result_info_number( global_voteinfo_code_for_number_of_equivalent_seats ) ;
        put_next_result_info_number( global_number_of_seats_to_fill ) ;
        if ( global_logging_info == global_true ) { log_out << "[number of equivalent seats to fill is " << global_number_of_seats_to_fill << "]" ; } ;
        return ;
    }


// -----------------------------------------------
//  If there is a request to ignore a candidate,
//  specify that it is already eliminated.  This
//  is useful for ignoring very unpopular
//  candidates as a way to reduce the number of
//  ballot groups and ballot patterns for
//  elections that have a very large number of
//  candidates.

    if ( global_previous_voteinfo_number == global_voteinfo_code_for_candidate_to_ignore )
    {
        candidate_number = global_current_voteinfo_number ;
        global_true_or_false_eliminated_candidate[ candidate_number ] = global_true ;
        put_next_result_info_number( global_voteinfo_code_for_eliminated_candidate ) ;
        put_next_result_info_number( candidate_number ) ;
        global_true_or_false_available_candidate[ candidate_number ] = global_false ;
        if ( global_logging_info == global_true ) { log_out << "[as requested, ignoring candidate number " << candidate_number << "]" ; } ;
        return ;
    }


// -----------------------------------------------
//  Ignore the value that follows the code for the
//  number of representation levels to calculate,
//  then return.

    if ( global_previous_voteinfo_number == global_voteinfo_code_for_number_of_representation_levels_to_compute )
    {
        return ;
    }


// -----------------------------------------------
//  If a positive number appears before the case
//  number, that is an error, so indicate it,
//  then return.

    if ( ( global_current_voteinfo_number > 0 ) && ( global_case_number < 1 ) )
    {
        if ( global_logging_info == global_true ) { log_out << "[error, positive number (" << global_current_voteinfo_number << ") encountered before case number specified]" ; } ;
        global_possible_error_message = "Error: Positive number (" + convert_integer_to_text( global_current_voteinfo_number ) + ") encountered before case number specified." ;
        return ;
    }


// -----------------------------------------------
//  Handle a candidate number.  It is specified as
//  as a voteinfo number that is greater than
//  zero and is not preceded by a negative code
//  that is otherwise handled.  Store the
//  candidate number in the ballot-specific info
//  list, check for errors, then return.

    if ( global_current_voteinfo_number > 0 )
    {
        candidate_number = global_current_voteinfo_number ;
        save_ballot_info_number( candidate_number ) ;
        global_count_of_candidates_marked ++ ;
        global_tally_uses_of_candidate_number[ candidate_number ] ++ ;
        if ( global_logging_info == global_true ) { log_out << "[candidate " << candidate_number << "]" ; } ;
        if ( global_number_of_candidates == 0 )
        {
            if ( global_logging_info == global_true ) { log_out << "[error, candidate number appears before number of candidates specified, at input line number " << global_input_line_number << "]" ; } ;
            global_possible_error_message = "Error: Candidate number appears before number of candidates specified, at input line number " + convert_integer_to_text( global_input_line_number ) + "." ;
            return ;
        }
        if ( candidate_number > global_number_of_candidates )
        {
            if ( global_logging_info == global_true ) { log_out << "[error, candidate number " << candidate_number << " exceeds indicated number of candidates, which is " << global_number_of_candidates << ", at input line number " << global_input_line_number << "]" ; } ;
            global_possible_error_message = "Error: Candidate number " + convert_integer_to_text( candidate_number ) + " exceeds indicated number of candidates, which is " + convert_integer_to_text( global_number_of_candidates ) + ", at input line number " + convert_integer_to_text( global_input_line_number ) + "." ;
            return ;
        }
        if ( global_tally_uses_of_candidate_number[ candidate_number ] > 1 )
        {
            if ( global_logging_info == global_true ) { log_out << "[error, candidate number " << candidate_number << " previously used in this ballot, error is at input line number " << global_input_line_number << "]" ; } ;
            global_possible_error_message = "Error: Candidate number " + convert_integer_to_text( candidate_number ) + " previously used in this ballot, error is at input line number " + convert_integer_to_text( global_input_line_number ) + "." ;
            return ;
        }
        return ;
    }


// -----------------------------------------------
//  If there is a request for no logging,
//  shut off logging now, then return.

    if ( global_current_voteinfo_number == global_voteinfo_code_for_request_logging_off )
    {
        global_logging_info = global_false ;
        return ;
    }


// -----------------------------------------------
//  If there is a request for the Droop quota
//  instead of the Hare quota, set a flag that is
//  used later, write this request to the results
//  info, then return.

    if ( global_current_voteinfo_number == global_voteinfo_code_for_request_quota_droop_not_hare )
    {
        put_next_result_info_number( global_voteinfo_code_for_request_quota_droop_not_hare ) ;
        global_true_or_false_request_quota_droop = global_true ;
        if ( global_logging_info == global_true ) { log_out << "[request for Droop quota instead of Hare quota]" ; } ;
        return ;
    }


// -----------------------------------------------
//  If there is a request for instant runoff
//  voting, interpret this request to not
//  eliminate pairwise losing candidates.  If
//  results for instant runoff voting are desired,
//  then also the number of available seats must
//  be specified as one.  Save this request in a
//  flag and write this request to the results
//  info, then return.

    if ( global_current_voteinfo_number == global_voteinfo_code_for_request_instant_runoff_voting )
    {
        global_true_or_false_request_no_pairwise_loser_elimination = global_true ;
        put_next_result_info_number( global_voteinfo_code_for_request_instant_runoff_voting ) ;
        if ( global_logging_info == global_true ) { log_out << "[request no pairwise loser eliminations]" ; } ;
        return ;
    }


// -----------------------------------------------
//  If there is a request to calculate the
//  primitive version of STV that ignores ballots
//  when counting reaches two or more candidates
//  sharing the same ranking level, save this
//  request in a flag and write this request to
//  the results info, then return.

    if ( global_current_voteinfo_number == global_voteinfo_code_for_request_ignore_shared_rankings )
    {
        global_true_or_false_request_ignore_shared_rankings = global_true ;
        put_next_result_info_number( global_voteinfo_code_for_request_ignore_shared_rankings ) ;
        if ( global_logging_info == global_true ) { log_out << "[request ignoring ballots when shared rankings are encountered]" ; } ;
        return ;
    }


// -----------------------------------------------
//  If the code for the end of all cases is
//  encountered without yet encountering a case
//  number, indicate this as an error, and return.

    if ( ( global_current_voteinfo_number == global_voteinfo_code_for_end_of_all_cases ) && ( global_case_number < 1 ) )
    {
        if ( global_logging_info == global_true ) { log_out << "[error, reached end of all cases, but no case has been started]" ; } ;
        global_possible_error_message = "Error: Reached end of all cases before case started." ;
        return ;
    }


// -----------------------------------------------
//  If a value of zero is encountered, indicate an
//  error, then return.

    if ( global_current_voteinfo_number == 0 )
    {
        log_out << "[error, encountered voteinfo value of zero, which is not valid]" ;
        global_possible_error_message = "Error: Encountered voteinfo value of zero, which is not valid." ;
        return ;
    }


// -----------------------------------------------
//  End of function handle_one_voteinfo_number.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//         read_data
//
//  Reads numbers, including codes (which are
//  negative numbers) from the standard input
//  file.  These numbers supply ballot data and
//  specify what to calculate and what results to
//  include in the results.  The codes that
//  specify ballot marks are saved in a list where
//  they can be accessed repeatedly.

void read_data( )
{

    int candidate_number ;
    int error_count ;

    std::string input_line ;
    std::string input_text_word ;


// -----------------------------------------------
//  Initialization.

    error_count = 0 ;
    for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
    {
        global_tally_uses_of_candidate_number[ candidate_number ] = 0 ;
    }


// -----------------------------------------------
//  Indicate the current status.

    if ( global_logging_info == global_true ) { log_out << "[about to start checking vote-info numbers]" << std::endl ; } ;


// -----------------------------------------------
//  Begin loop to handle one line from the input
//  file, which is "standard input" (which means
//  it's the input file specified on the command
//  line).
//
//  Although each code can be on a different line,
//  or all the codes and tokens can be on a single
//  line (if it is not too long), for clarity
//  each ballot can be on a separate line.

    for ( std::string input_line ; std::getline( std::cin , input_line ) ; )
    {
        global_input_line_number ++ ;
        std::size_t pointer_found = input_line.find_last_not_of( " \t\n\r" ) ;
        if ( pointer_found != std::string::npos )
        {
            input_line.erase( pointer_found + 1 ) ;
        } else
        {
            input_line.clear( ) ;
        }
        if ( global_logging_info == global_true ) { log_out << std::endl << "[input line: " << input_line << "]" ; } ;
        char input_line_c_version[ 2000 ] = "" ;
        std::size_t line_length = std::min( 2000 , (int) input_line.length() ) ;
        std::size_t line_length_copied = input_line.copy( input_line_c_version , line_length , 0 ) ;
        input_line_c_version[ line_length_copied ] = '\0' ;


// -----------------------------------------------
//  Begin loop to get first/next space-delimited
//  word (of text) from the input line.  It must
//  be an integer.

        char * pointer_to_word ;
        // reminder: strtok modifies the string
        pointer_to_word = strtok( input_line_c_version , " ,." ) ;
        while ( pointer_to_word != NULL )
        {
            input_text_word = pointer_to_word ;


// -----------------------------------------------
//  Attempt to convert the text word into an
//  integer.  If this conversion is not
//  successful, indicate an error.

            global_current_voteinfo_number = 0 ;
            try
            {
                global_current_voteinfo_number = convert_text_to_integer( pointer_to_word ) ;
            }
            catch( ... )
            {
                log_out << "[error, input line contains non-numeric characters: " << pointer_to_word << "]" << std::endl ;
                std::cout << "Error: Input line contains non-numeric characters: " << pointer_to_word << std::endl ;
                error_count ++ ;
            }


// -----------------------------------------------
//  Handle one voteinfo number, which is either a
//  code expressed as a negative integer, or is a
//  positive integer.  If the text is not an
//  integer, or if the text is the integer zero,
//  ignore it.

            if ( global_current_voteinfo_number != 0 )
            {
                handle_one_voteinfo_number( ) ;
            }


// -----------------------------------------------
//  If there are errors, report them.  If there
//  are lots of errors, exit extra early.

            if ( global_possible_error_message.length() > 3 )
            {
                log_out << std::endl << "[error, message is: " << global_possible_error_message << "]" << std::endl ;
                std::cout << "Error, error message is: " << global_possible_error_message << std::endl ;
                error_count ++ ;
                global_possible_error_message = "" ;
            }
            if ( error_count > 10 )
            {
                log_out << std::endl << "[too many errors to log, exiting early]" << std::endl ;
                std::cout << "Too many errors to log, exiting early." << std::endl ;
                exit( EXIT_FAILURE ) ;
            }


// -----------------------------------------------
//  Repeat the loop for the next word (within the line).

            global_previous_voteinfo_number = global_current_voteinfo_number ;
            pointer_to_word = strtok( NULL, " ,." ) ;
        }


// -----------------------------------------------
//  Repeat the loop for the next line of data from
//  the input file.

    }
    if ( global_logging_info == global_true ) { log_out << "[done getting input data]" << std::endl ; }


// -----------------------------------------------
//  If there were any data errors, exit now.

    if ( error_count > 0 )
    {
        log_out << std::endl << "[encountered one or more fatal data-input errors]" << std::endl ;
        std::cout << "Encountered one or more fatal data-input errors, exiting now." << std::endl ;
        exit( EXIT_FAILURE ) ;
    }


// -----------------------------------------------
//  Log the number of ballot groups.

    if ( global_logging_info == global_true ) { log_out << "[" << global_total_count_of_ballot_groups << " ballot groups]" << std::endl ; } ;


// -----------------------------------------------
//  Put into the voteinfo ballot codes the code
//  that indicates the end of the ballots.

    global_pointer_to_voteinfo_number ++ ;
    global_vote_info_list[ global_pointer_to_voteinfo_number ] = global_voteinfo_code_for_end_of_all_vote_info ;


// -----------------------------------------------
//  Specify the length of the list that now holds
//  the ballot data.

    global_pointer_to_end_of_voteinfo_numbers = global_pointer_to_voteinfo_number ;


// -----------------------------------------------
//  If a meaningful number of ballots were not
//  found, indicate this no-data error.

    if ( global_total_count_of_ballot_groups < 2 )
    {
        if ( global_logging_info == global_true ) { log_out << "[error, input file does not contain any ballot data]" ; } ;
        std::cout << "Error, input file does not contain any ballot data." << std::endl ;
        exit( EXIT_FAILURE ) ;
    }


// -----------------------------------------------
//  End of function read_data.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//    point_to_next_ballot_group
//
//  Points to the next ballot group without
//  getting any info from the skipped group of
//  identical ballots.

void point_to_next_ballot_group( )
{


// -----------------------------------------------
//  If the pointer is already at the end of the
//  ballots, return.

    if ( global_pointer_to_voteinfo_number >= global_pointer_to_end_of_voteinfo_numbers )
    {
        return ;
    }


// -----------------------------------------------
//  Skip the ballot repeat count.

    global_pointer_to_voteinfo_number ++ ;
    global_pointer_to_voteinfo_number ++ ;


// -----------------------------------------------
//  Loop through the voteinfo codes until the
//  beginning of the next ballot is reached.

    while ( global_pointer_to_voteinfo_number < global_pointer_to_end_of_voteinfo_numbers )
    {
        global_current_voteinfo_number = global_vote_info_list[ global_pointer_to_voteinfo_number ] ;
        if ( ( global_current_voteinfo_number == global_voteinfo_code_for_ballot_count ) || ( global_current_voteinfo_number == global_voteinfo_code_for_end_of_all_vote_info ) )
        {
            break ;
        }
        global_pointer_to_voteinfo_number ++ ;
    }


// -----------------------------------------------
//  End of function point_to_next_ballot_group.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//    get_candidate_ranks_from_one_ballot_group
//
//  Gets the candidate rankings from the next
//  ballot group.  Every ballot in the group has
//  the same specified preferences.  This grouping
//  allows them to be processed together.

int get_candidate_ranks_from_one_ballot_group( )
{

    int candidate_number ;
    int preference_level ;

    std::string text_ballot_info ;


// -----------------------------------------------
//  If the pointer is already at the end of the
//  ballots, return with a value of zero.

    if ( global_pointer_to_voteinfo_number >= global_pointer_to_end_of_voteinfo_numbers )
    {
        global_ballot_info_repeat_count = 0 ;
        log_out << "[voteinfo pointer has already reached the end of the ballots]" << std::endl ;
        return 0 ;
    }


// -----------------------------------------------
//  If the voteinfo pointer does not already point
//  to the ballot repeat count, that is a fatal
//  error.

    if ( global_vote_info_list[ global_pointer_to_voteinfo_number ] != global_voteinfo_code_for_ballot_count )
    {
        log_out << "[error, bug has been introduced into code, expected ballot repeat count voteinfo code, but found something else]" << std::endl ;
        std::cout << "Error: Bug has been introduced into code, expected ballot repeat count voteinfo code, but found something else.]" << std::endl ;
        exit( EXIT_FAILURE ) ;
    }


// -----------------------------------------------
//  If the number of ballot groups exceeds the
//  assigned storage capacity, indicate a fatal
//  error.

    if ( global_ballot_group_pointer >= global_maximum_number_of_ballot_groups )
    {
        if ( global_logging_info == global_true ) { log_out << "[error, number of ballot groups exceeds assigned storage capacity limit, which is " << global_maximum_number_of_ballot_groups << "]" << std::endl ; } ;
        log_out << "[Error: Number of ballot groups exceeds assigned storage capacity limit, which is " << global_maximum_number_of_ballot_groups << "]" << std::endl ;
        std::cout << "Error: Number of ballot groups exceeds assigned storage capacity limit, which is " << convert_integer_to_text( global_maximum_number_of_ballot_groups ) << ".]" << std::endl ;
        exit( EXIT_FAILURE ) ;
    }


// -----------------------------------------------
//  In case any of the candidate numbers are not
//  encountered, initialize all the preference
//  levels to a very low ranking level.

    for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
    {
        global_ballot_preference_for_candidate[ candidate_number ] = 200 ;
    }


// -----------------------------------------------
//  Get the ballot repeat count, which is the
//  number of ballots that are marked with the
//  same preference pattern.

    global_pointer_to_voteinfo_number ++ ;
    global_ballot_info_repeat_count = global_vote_info_list[ global_pointer_to_voteinfo_number ] ;
    global_pointer_to_voteinfo_number ++ ;


// -----------------------------------------------
//  If this is the first time getting the ballot
//  count for this group, save this ballot count
//  as the initial influence count for this ballot
//  group.

    if ( global_need_to_initialize_group_ballot_count == global_true )
    {
        global_ballot_count_remaining_for_ballot_group[ global_ballot_group_pointer ] = global_ballot_info_repeat_count ;
    }


// -----------------------------------------------
//  Begin a loop that gets the voteinfo codes and
//  candidate numbers within the ballot.

    preference_level = 1 ;
    while ( global_pointer_to_voteinfo_number <= global_pointer_to_end_of_voteinfo_numbers )
    {


// -----------------------------------------------
//  Get the current voteinfo number.

        global_current_voteinfo_number = global_vote_info_list[ global_pointer_to_voteinfo_number ] ;
//        if ( global_logging_info == global_true ) { log_out << "[" << global_pointer_to_voteinfo_number << ": " << global_current_voteinfo_number << "]" ; } ;


// -----------------------------------------------
//  If the end of the ballot has been reached,
//  leave the pointer pointing to the beginning of
//  the next ballot, and exit the loop.

        if ( ( global_current_voteinfo_number == global_voteinfo_code_for_ballot_count ) || ( global_current_voteinfo_number == global_voteinfo_code_for_end_of_all_vote_info ) )
        {
            break ;
        }


// -----------------------------------------------
//  Adjust the preference level at tied levels.

        if ( global_current_voteinfo_number == global_voteinfo_code_for_tie )
        {
            preference_level -- ;
            text_ballot_info += " tie" ;
        }


// -----------------------------------------------
//  For a candidate number, determine its preference
//  level.

        if ( global_current_voteinfo_number > 0 )
        {
            candidate_number = global_current_voteinfo_number ;
            global_ballot_preference_for_candidate[ candidate_number ] = preference_level ;
            text_ballot_info += " " + convert_integer_to_text( candidate_number ) ;
            preference_level ++ ;
        }


// -----------------------------------------------
//  Repeat the loop to handle the next
//  voteinfo number.

        global_pointer_to_voteinfo_number ++ ;
    }


// -----------------------------------------------
//  Log the current ballot count and candidate
//  ranking sequence.

    if ( global_logging_info == global_true ) { log_out << "[group " << global_ballot_group_pointer << " bc " << global_ballot_info_repeat_count << " r " << global_ballot_count_remaining_for_ballot_group[ global_ballot_group_pointer ] << " " << text_ballot_info << "]" << std::endl ; } ;


// -----------------------------------------------
//  Return with the number of ballot counts
//  handled.  If zero, there are no more ballots.

    return global_ballot_info_repeat_count ;


// -----------------------------------------------
//  End of function get_candidate_ranks_from_one_ballot_group.

}


// -----------------------------------------------
// -----------------------------------------------
//          reset_tally_table
//
//  Create and initialize the tally table.

void reset_tally_table( )
{

    int pair_counter ;
    int first_candidate_number ;
    int second_candidate_number ;


// -----------------------------------------------
//  Create the pairwise candidate counters.  If
//  the storage limit is exceeded, indicate this
//  by putting zero into the value
//  global_pair_counter_maximum.

    pair_counter = 0 ;
    global_pair_counter_maximum = 0 ;
    for ( first_candidate_number = 1 ; first_candidate_number < global_number_of_candidates ; first_candidate_number ++ )
    {
        if ( global_true_or_false_pairwise_consider_candidate[ first_candidate_number ] == global_true )
        {
            for ( second_candidate_number = first_candidate_number + 1 ; second_candidate_number <= global_number_of_candidates ; second_candidate_number ++ )
            {
                if ( global_true_or_false_pairwise_consider_candidate[ second_candidate_number ] == global_true )
                {
                    pair_counter ++ ;
                    global_pair_counter_maximum ++ ;
                    if ( global_pair_counter_maximum >= global_maximum_candidate_pairs )
                    {
                        global_pair_counter_maximum = 0 ;
                        if ( global_logging_info == global_true ) { log_out << "[error, the number of candidates being pairwise counted exceeds the available storage space]" ; } ;
                        return ;
                    }
                    global_first_candidate_number_in_pair[ pair_counter ] = first_candidate_number ;
                    global_second_candidate_number_in_pair[ pair_counter ] = second_candidate_number ;
                }
            }
        }
    }


// -----------------------------------------------
//  If there were no candidates to pairwise
//  compare, return with zero in the variable
//  global_pair_counter_maximum.

    if ( global_pair_counter_maximum == 0 )
    {
        return ;
    }


// -----------------------------------------------
//  Create, and clear, the tally table.

    for ( pair_counter = 1 ; pair_counter <= global_pair_counter_maximum ; pair_counter ++ )
    {
        global_tally_first_over_second_in_pair[ pair_counter ] = 0 ;
        global_tally_second_over_first_in_pair[ pair_counter ] = 0 ;
        global_tally_first_equal_second_in_pair[ pair_counter ] = 0 ;
    }


// -----------------------------------------------
//  End of function reset_tally_table.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//        fill_pairwise_tally_table
//
//  This function fills the tally table with
//  pairwise counts that apply to the candidates
//  specified as still available to get elected.
//
//  When a candidate gets elected the ballot
//  influence numbers change, so that changes the
//  numbers in the tally table.  When a candidate
//  is eliminated, a new tally table must be
//  created and filled because one of the
//  candidates is no longer among the pairs being
//  tallied.

void fill_pairwise_tally_table( )
{

    int pair_counter ;
    int candidate_number ;
    int first_candidate_number ;
    int second_candidate_number ;
    int remaining_ballot_count_for_current_ballot_group ;


// -----------------------------------------------
//  Log the number of pairs.

    if ( global_logging_info == global_true ) { log_out << "[pair count is " << global_pair_counter_maximum << "]" << std::endl ; } ;


// -----------------------------------------------
//  Begin a loop that handles each ballot group.

    global_pointer_to_voteinfo_number = 1 ;
    for ( global_ballot_group_pointer = 1 ; global_ballot_group_pointer <= global_total_count_of_ballot_groups ; global_ballot_group_pointer ++ )
    {


// -----------------------------------------------
//  Get the number of remaining ballots in this
//  ballot group that still have full influence.

        remaining_ballot_count_for_current_ballot_group = global_ballot_count_remaining_for_ballot_group[ global_ballot_group_pointer ] ;
//        if ( global_logging_info == global_true ) { log_out << "[ballot group " << global_ballot_group_pointer << " has " << remaining_ballot_count_for_current_ballot_group << " ballots]" << std::endl ; } ;


// -----------------------------------------------
//  If none of the ballots in this ballot group
//  have their full influence, restart the loop to
//  handle the next ballot group.

        if ( remaining_ballot_count_for_current_ballot_group < 1 )
        {
            log_out << "[all ballots in ballot group " << global_ballot_group_pointer << " have no more influence]" << std::endl ;
            continue ;
        }


// -----------------------------------------------
//  Get the preferences on the ballots in this
//  group of identical ballots.

        global_ballot_info_repeat_count = get_candidate_ranks_from_one_ballot_group( ) ;


// -----------------------------------------------
//  For the current ballot pattern, add the
//  pairwise counts to the tally table.  The pair
//  counter numbers already limit the pairs to the
//  candidates who are still being considered.
//  Reminder: The preference level numbers are
//  smallest for the highest-ranked candidates.

        for ( pair_counter = 1 ; pair_counter <= global_pair_counter_maximum ; pair_counter ++ )
        {
            first_candidate_number = global_first_candidate_number_in_pair[ pair_counter ] ;
            second_candidate_number = global_second_candidate_number_in_pair[ pair_counter ] ;
            if ( global_ballot_preference_for_candidate[ first_candidate_number ] < global_ballot_preference_for_candidate[ second_candidate_number ] )
            {
                global_tally_first_over_second_in_pair[ pair_counter ] += remaining_ballot_count_for_current_ballot_group ;
            } else if ( global_ballot_preference_for_candidate[ first_candidate_number ] > global_ballot_preference_for_candidate[ second_candidate_number ] )
            {
                global_tally_second_over_first_in_pair[ pair_counter ] += remaining_ballot_count_for_current_ballot_group ;
            } else
            {
                global_tally_first_equal_second_in_pair[ pair_counter ] += remaining_ballot_count_for_current_ballot_group ;
            }
        }


// -----------------------------------------------
//  Repeat the loop that gets info from each group
//  of identical ballots.

    }


// -----------------------------------------------
//  End of function fill_pairwise_tally_table.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//        check_for_pairwise_losing_candidate
//
//  This function looks for a pairwise losing
//  candidate, and identifies that candidate if
//  it exists.  It is impossible for there to be
//  more than one pairwise losing candidate.

int check_for_pairwise_losing_candidate( )
{

    int pair_counter ;
    int candidate_number ;
    int first_candidate_number ;
    int second_candidate_number ;
    int number_of_candidates_being_pairwise_considered ;


// -----------------------------------------------
//  Initialize the loss count numbers.  Also count
//  how many candidates are being considered in
//  this comparison, which can differ from the
//  number of remaining candidates because
//  pairwise comparisons are also used to break
//  ties.

    number_of_candidates_being_pairwise_considered = 0 ;
    for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
    {
        global_loss_count_for_candidate[ candidate_number ] = 0 ;
        if ( global_true_or_false_pairwise_consider_candidate[ candidate_number ] == global_true )
        {
            number_of_candidates_being_pairwise_considered ++ ;
        }
    }


// -----------------------------------------------
//  Count the number of pairwise losses for each
//  candidate.

    for ( pair_counter = 1 ; pair_counter <= global_pair_counter_maximum ; pair_counter ++ )
    {
        first_candidate_number = global_first_candidate_number_in_pair[ pair_counter ] ;
        second_candidate_number = global_second_candidate_number_in_pair[ pair_counter ] ;
        if ( global_tally_first_over_second_in_pair[ pair_counter ] > global_tally_second_over_first_in_pair[ pair_counter ] )
        {
            global_loss_count_for_candidate[ second_candidate_number ] ++ ;
//            if ( global_logging_info == global_true ) { log_out << "[candidate " << second_candidate_number << " loses to candidate " << first_candidate_number << ", with " << global_tally_second_over_first_in_pair[ pair_counter ] << " votes versus " << global_tally_first_over_second_in_pair[ pair_counter ] << " votes]" << std::endl ; } ;
        } else if ( global_tally_second_over_first_in_pair[ pair_counter ] > global_tally_first_over_second_in_pair[ pair_counter ] )
        {
            global_loss_count_for_candidate[ first_candidate_number ] ++ ;
//            if ( global_logging_info == global_true ) { log_out << "[candidate " << first_candidate_number << " loses to candidate " << second_candidate_number << ", with " << global_tally_first_over_second_in_pair[ pair_counter ] << " votes versus " << global_tally_second_over_first_in_pair[ pair_counter ] << " votes]" << std::endl ; } ;
        } else
        {
//            if ( global_logging_info == global_true ) { log_out << "[equal vote counts for candidate " << first_candidate_number << " and candidate " << second_candidate_number << ", both have " << global_tally_first_over_second_in_pair[ pair_counter ] << " votes]" << std::endl ; } ;
        }
    }


// -----------------------------------------------
//  If there is a pairwise losing candidate,
//  return with its candidate number.

    if ( global_logging_info == global_true ) { log_out << "[pairwise comparing " << number_of_candidates_being_pairwise_considered << " candidates]" << std::endl ; } ;
    for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
    {
        if ( global_true_or_false_pairwise_consider_candidate[ candidate_number ] == global_true )
        {
            if ( global_loss_count_for_candidate[ candidate_number ] > 0 )
            {
//                if ( global_logging_info == global_true ) { log_out << "[loss count for candidate " << candidate_number << " is " << global_loss_count_for_candidate[ candidate_number ] << "]" << std::endl ; } ;
                if ( global_loss_count_for_candidate[ candidate_number ] == ( number_of_candidates_being_pairwise_considered - 1 ) )
                {
                    if ( global_logging_info == global_true ) { log_out << "[found pairwise losing candidate, candidate " << candidate_number << "]" << std::endl ; } ;
                    return candidate_number ;
                }
            }
        }
    }


// -----------------------------------------------
//  There was not pairwise losing candidate, so
//  return with zero in what otherwise would be
//  the candidate number.

    if ( global_logging_info == global_true ) { log_out << "[did not find pairwise losing candidate]" << std::endl ; } ;
    return 0 ;


// -----------------------------------------------
//  End of function check_for_pairwise_losing_candidate.

}


// -----------------------------------------------
// -----------------------------------------------
//   identify_top_ranked_candidates
//
//  Identify which candidate is, or which
//  candidates are, ranked at the top of the
//  identical ballots in the current ballot
//  group.  Already-elected and already-eliminated
//  candidates are ignored.

void identify_top_ranked_candidates( )
{

    int candidate_number ;
    int top_ranked_candidate_if_only_one_else_zero ;
    int highest_preference_level_of_any_remaining_candidate ;
    int pointer_to_list_of_candidates_with_highest_transfer_count ;


// -----------------------------------------------
//  Initialization.

    top_ranked_candidate_if_only_one_else_zero = 0 ;
    global_count_of_top_ranked_remaining_candidates = 0 ;
    highest_preference_level_of_any_remaining_candidate = 0 ;


// -----------------------------------------------
//  Begin a loop that handles each candidate who
//  is still available to win a seat.

    for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
    {
        global_true_or_false_is_top_ranked_candidate[ candidate_number ] = global_false ;
        if ( global_true_or_false_available_candidate[ candidate_number ] == global_true )
        {


// -----------------------------------------------
//  Identify which candidates are top ranked.
//  Save the top ranked candidate number for cases
//  in which there is just one top-ranked
//  candidate, and save the list of top-ranked
//  candidates for cases in which there are
//  multiple candidates.
//
//  Reminder: Ranking level 1 is the highest
//  ranking level, and ranking levels with higher
//  numbers are lower-ranked.

            if ( ( highest_preference_level_of_any_remaining_candidate == 0 ) || ( global_ballot_preference_for_candidate[ candidate_number ] < highest_preference_level_of_any_remaining_candidate  ) )
            {
                highest_preference_level_of_any_remaining_candidate = global_ballot_preference_for_candidate[ candidate_number ] ;
                global_count_of_top_ranked_remaining_candidates = 1 ;
                global_list_of_top_ranked_candidates[ global_count_of_top_ranked_remaining_candidates ] = candidate_number ;
                global_top_ranked_candidate_for_ballot_group[ global_ballot_group_pointer ] = candidate_number ;
                top_ranked_candidate_if_only_one_else_zero = candidate_number ;
            } else if ( global_ballot_preference_for_candidate[ candidate_number ] == highest_preference_level_of_any_remaining_candidate  )
            {
                global_count_of_top_ranked_remaining_candidates ++ ;
                global_list_of_top_ranked_candidates[ global_count_of_top_ranked_remaining_candidates ] = candidate_number ;
                top_ranked_candidate_if_only_one_else_zero = 0 ;
                global_top_ranked_candidate_for_ballot_group[ global_ballot_group_pointer ] = 0 ;
            }


// -----------------------------------------------
//  Repeat the loop for the next available
//  candidate.

        }
    }


// -----------------------------------------------
//  Create flags that indicate whether each
//  candidate is one of the top-ranked candidates.

    for ( pointer_to_list_of_candidates_with_highest_transfer_count = 1 ; pointer_to_list_of_candidates_with_highest_transfer_count <= global_count_of_top_ranked_remaining_candidates ; pointer_to_list_of_candidates_with_highest_transfer_count ++ )
    {
        candidate_number = global_list_of_top_ranked_candidates[ pointer_to_list_of_candidates_with_highest_transfer_count ] ;
        global_true_or_false_is_top_ranked_candidate[ candidate_number ] = global_true ;
//        if ( global_logging_info == global_true ) { log_out << "[candidate " << candidate_number << " is top-ranked]" << std::endl ; } ;
    }


// -----------------------------------------------
//  End of function identify_top_ranked_candidates.

}


// -----------------------------------------------
// -----------------------------------------------
//   add_current_ballot_group_votes_to_vote_transfer_counts
//
//  Based on the rankings from one group of identical
//  ballots, identify which candidate, or
//  candidates, are top ranked, and add the group's
//  ballot count to the counter that tracks ballots
//  that have the same top-ranked candidates.
//
//  Fuller explanation: If a ballot ranks two or
//  more remaining (not-yet-elected and
//  not-yet-eliminated) candidates at the same
//  preference level, and if there are not any
//  remaining candidates ranked higher on this
//  ballot, then this ballot is grouped with other
//  similar, although not necessarily identical,
//  ballots and their influence counts are equally
//  split among the remaining candidates who are
//  ranked at that shared preference level.
//
//  For example, if candidates A and B have been
//  elected or eliminated, and a ballot ranks
//  candidate A highest and ranks candidates
//  B, C, and D at the next-highest level, and
//  another ballot ranks candidate B highest and
//  ranks candidates A, C, and D at the next-highest
//  level, then one of these two ballots transfers
//  to candidate C and the other ballot transfers
//  to candidate D.  The choice of which ballot
//  transfers to which candidate must be handled so
//  that supplying the same ballots in a different
//  sequence is extremely unlikely to produce a
//  different election result.

void add_current_ballot_group_votes_to_vote_transfer_counts( )
{

    int candidate_number ;
    int pattern_number ;
    int pattern_number_pointer ;
    int remaining_pattern_number ;
    int pointer_to_matching_pattern_number ;
    int highest_preference_level_of_any_remaining_candidate ;
    int pointer_to_list_of_candidates_with_highest_transfer_count ;
    int pointer_to_list_of_candidates_with_lowest_transfer_count ;


// -----------------------------------------------
//  If this ballot group has no more influence,
//  return.

    if ( global_ballot_count_remaining_for_ballot_group[ global_ballot_group_pointer ] <= 0 )
    {
    	return ;
    }


// -----------------------------------------------
//  Identify which candidate, or candidates, are
//  top ranked, which means higher than the
//  remaining (not-yet-elected and
//  not-yet-eliminated) candidates.

    identify_top_ranked_candidates( ) ;


// -----------------------------------------------
//  If there are too many top-ranked candidates,
//  clear the count to zero.
//
//  The limit is 5 because 6 candidates will
//  create a ballot pattern number that is an
//  excessively large integer.

    if ( global_count_of_top_ranked_remaining_candidates > 5 )
    {
        global_count_of_top_ranked_remaining_candidates = 0 ;
        if ( global_logging_info == global_true ) { log_out << "[too many top-ranked candidates (" << global_count_of_top_ranked_remaining_candidates << ") in ballot group " << global_count_of_top_ranked_remaining_candidates << " so ignoring this ballot group during this counting cycle]" << std::endl ; } ;
    }


// -----------------------------------------------
//  If this ballot marks more than one top-ranked
//  candidate at the same ranking level, and if
//  there is a request to simulate the primitive
//  version of STV that ignores such ballots when
//  counting reaches the shared ranking level,
//  change this ballot group to have zero
//  influence.
//  A later counting cycle might no longer have
//  this sharing during a later counting cycle
//  (because some of the candidates get
//  eliminated), yet this complete rejection of
//  the ballots simulates the primitive version of
//  STV that fully rejects such ballots.

    if ( ( global_true_or_false_request_ignore_shared_rankings == global_true ) && ( global_count_of_top_ranked_remaining_candidates > 1 ) )
    {
    	global_ballot_count_remaining_for_ballot_group[ global_ballot_group_pointer ] = 0 ;
    	global_count_of_top_ranked_remaining_candidates = 0 ;
        log_out << "[ballot group " << global_ballot_group_pointer << ", influence is now zero because of shared ranking level and request to use primitive version of STV]" << std::endl ;
        return ;
    }


// -----------------------------------------------
//  Calculate a pattern number that is unique for
//  the combination of shared top ranked
//  candidates.  If only one candidate is top
//  ranked, the pattern number is simply the
//  candidate number.  This pattern-number
//  generation method accesses the candidate
//  numbers in candidate-number sequence so that
//  the same candidates (accessed in any order)
//  yield the same pattern number.  If the pattern
//  number is zero, exit with an error message.

    pattern_number = 0 ;
    if ( global_count_of_top_ranked_remaining_candidates > 0 )
    {
        for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
        {
            if ( global_true_or_false_is_top_ranked_candidate[ candidate_number ] == global_true )
            {
                pattern_number = ( pattern_number * ( global_number_of_candidates + 1 ) ) + candidate_number ;
            }
        }
    }
    if ( pattern_number == 0 )
    {
        log_out << "[error, bug has been introduced into code, pattern number is zero]" ;
        std::cout << "[Error: Bug has been introduced into code, pattern number is zero.]" << std::endl ;
        exit( EXIT_FAILURE ) ;
    }


// -----------------------------------------------
//  If this pattern number has been encountered
//  previously, point to it. Otherwise set the
//  pointer to zero.

    pointer_to_matching_pattern_number = 0 ;
    if ( global_count_of_unique_pattern_numbers > 0 )
    {
        for ( pattern_number_pointer = 1 ; pattern_number_pointer <= global_count_of_unique_pattern_numbers ; pattern_number_pointer ++ )
        {
            if ( pattern_number == global_pattern_number_for_pattern_number_pointer[ pattern_number_pointer ] )
            {
                pointer_to_matching_pattern_number = pattern_number_pointer ;
                break ;
            }
        }
    }


// -----------------------------------------------
//  Keep track of how many ballots have the same
//  candidates sharing the top/highest ranking
//  level.

    if ( pointer_to_matching_pattern_number > 0 )
    {
        global_ballot_count_for_pattern_number_pointer[ pointer_to_matching_pattern_number ] += global_ballot_count_remaining_for_ballot_group[ global_ballot_group_pointer ] ;
    } else
    {
        if ( global_count_of_unique_pattern_numbers < global_maximum_number_of_pattern_numbers )
        {
            global_count_of_unique_pattern_numbers ++ ;
            pointer_to_matching_pattern_number = global_count_of_unique_pattern_numbers ;
            global_pattern_number_for_pattern_number_pointer[ pointer_to_matching_pattern_number ] = pattern_number ;
            global_ballot_count_for_pattern_number_pointer[ pointer_to_matching_pattern_number ] = global_ballot_count_remaining_for_ballot_group[ global_ballot_group_pointer ] ;
            global_top_candidate_count_for_pattern_number_pointer[ pointer_to_matching_pattern_number ] = global_count_of_top_ranked_remaining_candidates ;
        } else
        {
            log_out << "[error: there are too many unique patterns of ballot markings that rank multiple remaining candidates at the same preference level, so increase the length of the lists named global_pattern_number_for_pattern_number_pointer, global_ballot_count_for_pattern_number_pointer, and global_top_candidate_count_for_pattern_number_pointer, which currently have a length of " << global_maximum_number_of_pattern_numbers << "]" << std::endl ;
            std::cout << "[Error: There are too many unique patterns of ballot markings that rank multiple remaining candidates at the same preference level, so increase the length of the lists named global_pattern_number_for_pattern_number_pointer, global_ballot_count_for_pattern_number_pointer, and global_top_candidate_count_for_pattern_number_pointer, which currently have a length of " << global_maximum_number_of_pattern_numbers << ".]" << std::endl ;
            exit( EXIT_FAILURE ) ;
        }
    }


// -----------------------------------------------
//  End of function add_current_ballot_group_votes_to_vote_transfer_counts.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//  calculate_transfer_count_for_each_candidate
//
//  This function calculates the vote transfer
//  count for every remaining (not-yet-elected and
//  not-yet-eliminated) candidate.  When multiple
//  candidates are ranked at the same highest
//  preference level, keep track of which ballots
//  (by ballot group and ballot count within that
//  group) transfer to which candidate.  This
//  approach ensures that ballots are allocated in
//  whole (integer) numbers and equal numbers to
//  avoid giving any ballot a fractional or
//  decimal influence amount.

void calculate_transfer_count_for_each_candidate( )
{

    int candidate_number ;
    int pattern_number ;
    int remaining_pattern_number ;
    int pattern_number_pointer ;
    int count_of_candidates_at_top_preference_level ;
    int ballot_count_for_shared_preference_level ;
    int count_of_votes_to_each_candidate_at_shared_preference_level ;


// -----------------------------------------------
//  Clear the total vote count to restart this
/// count, which can change because some ballots
//  are changed to zero influence after they help
//  to elect a candidate.

    global_current_total_vote_count = 0 ;


// -----------------------------------------------
//  Begin a loop that handles each pattern number.

    for ( pattern_number_pointer = 1 ; pattern_number_pointer <= global_count_of_unique_pattern_numbers ; pattern_number_pointer ++ )
    {


// -----------------------------------------------
//  Get the counts from the groups of ballots that
//  ranked the same candidates at the same top
//  preference level.

        ballot_count_for_shared_preference_level = global_ballot_count_for_pattern_number_pointer[ pattern_number_pointer ] ;
        remaining_pattern_number = global_pattern_number_for_pattern_number_pointer[ pattern_number_pointer ] ;
        count_of_candidates_at_top_preference_level = global_top_candidate_count_for_pattern_number_pointer[ pattern_number_pointer ] ;


// -----------------------------------------------
//  Calculate how many votes go to each remaining
//  candidate if multiple candidates are ranked at
//  the same highest preference level.

        count_of_votes_to_each_candidate_at_shared_preference_level = int( float( ballot_count_for_shared_preference_level ) / float( count_of_candidates_at_top_preference_level ) ) ;


// -----------------------------------------------
//  If this ballot group now has more than one
//  top-ranked candidate, log the info for this
//  adjusted ballot group.

        if ( count_of_candidates_at_top_preference_level > 1 )
        {
            if ( global_logging_info == global_true ) { log_out << "[check: pattern number " << remaining_pattern_number << ", shared pref count " << count_of_candidates_at_top_preference_level << ", ballot count " << ballot_count_for_shared_preference_level << ", each candidate gets " << count_of_votes_to_each_candidate_at_shared_preference_level << " votes]" << std::endl ; } ;
        }


// -----------------------------------------------
//  Transfer the just-calculated vote count to
//  each top-ranked candidate -- for the ballots
//  that have this top-ranking pattern.  This is
//  the transfer count that is used to elect
//  candidates, and (when there are no pairwise
//  losing candidates) eliminate candidates.
//
//  Reminder:  The "%" symbol requests "modulo"
//  arithmetic.

        while ( remaining_pattern_number > 0 )
        {
            candidate_number = remaining_pattern_number % ( global_number_of_candidates + 1 ) ;
            remaining_pattern_number = ( remaining_pattern_number - candidate_number ) / ( global_number_of_candidates + 1 ) ;
            global_vote_transfer_count_for_candidate[ candidate_number ] += count_of_votes_to_each_candidate_at_shared_preference_level ;
            global_current_total_vote_count += count_of_votes_to_each_candidate_at_shared_preference_level ;
//            if ( global_logging_info == global_true ) { log_out << "[candidate " << candidate_number << " gets " << count_of_votes_to_each_candidate_at_shared_preference_level << " votes]" << std::endl ; } ;
        }


// -----------------------------------------------
//  Repeat the loop that handles each pattern number.

    }


// -----------------------------------------------
//  End of function calculate_transfer_count_for_each_candidate.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//        adjust_for_quota_excess
//
//  After a candidate has been elected, reduce to
//  zero influence some or most of the supporting
//  ballots that caused that candidate to win.
//  Retain influence for some ballots to the
//  extent that the transfer count exceeded the
//  quota count.
//
//  These adjustments change the weight of some
//  elected-candidate-supporting ballots to have
//  zero influence.  The other ballots continue to
//  have full influence.  This adjustment is done
//  to comply with some laws that do not allow a
//  ballot to have fractional (or decimal, or
//  partial) influence (between zero and full
//  influence).  These adjustments yield the same
//  results as if fractional ballot weighting was
//  used.
//
//  To appreciate why all the supporting ballots
//  do not get zero influence, consider this case.
//  Imagine that all the voters rank the
//  first-seat winner at the top of their ballot.
//  If all those ballots were reduced to zero
//  influence, that would leave no ballots to
//  determine which candidates deserve to win the
//  other seats.
//
//  The amount of influence retained by the
//  supporting ballots is larger when the number
//  of supporting ballots go far beyond the quota
//  count.  If, instead, the supporting ballots
//  are only sufficient to barely reach the quota
//  count, then there is no excess support, and
//  all the supporting ballots are reduced to
//  zero influence.
//
//  The specific supporting ballots that are
//  changed from full influence to zero influence
//  are chosen to be equally spaced from one
//  another in the supplied ballot sequence,
//  without including the already-zero-influence
//  ballots in the equal-spacing calculations.
//
//  These adjustments ensure, but do not
//  guarantee, that a different supplied ballot
//  sequence is extremely unlikely to change
//  which candidates win the seats.

void adjust_for_quota_excess( )
{

    int remaining_ballot_count_for_current_ballot_group ;
    int top_ranked_candidate_if_only_one_else_zero ;
    int number_of_ballots_after_reduced_influence ;
    int pattern_number ;
    int pointer_to_matching_pattern_number ;
    int gap_between_original_and_adjusted ;
    int candidate_number ;
    int number_of_ballots_getting_zero_influence ;
    int total_number_of_ballots_that_got_zero_influence ;


// -----------------------------------------------
//  Declare two decimal variables that together
//  are used to identify which ballots are equally
//  spaced among the ballots that support the
//  just-elected candidate.

    float decimal_skip_interval_size ;
    float decimal_residual_vote_transfer_count ;


// -----------------------------------------------
//  Initialization.

    total_number_of_ballots_that_got_zero_influence = 0 ;
    decimal_residual_vote_transfer_count = 0.0 ;


// -----------------------------------------------
//  Log the quota and excess support counts.

    if ( global_logging_info == global_true ) { log_out << std::endl << "[adjusting vote counts, winning candidate " << global_candidate_just_elected << ", quota is " << global_quota_count << ", excess support is " << global_supporting_vote_count_that_exceeds_quota << "]" << std::endl ; } ;


// -----------------------------------------------
//  Calculate one plus the number of supporting
//  ballots that are skipped when choosing which
//  supporting ballots are reduced to zero
//  influence.  If there were no excess votes
//  beyond the quota count, choose a skip interval
//  that will skip all the supporting ballots.
//  This number is a decimal value because an
//  integer interval typically would be too
//  large or too small, and therefore would
//  identify too many or too few supporting
//  ballots that need their influence changed to
//  zero.  This decimal number is NOT used to
//  calculate a ballot's adjusted influence
//  because a ballot can only have full influence
//  or no influence, not partial influence.

    if ( global_quota_count > 0 )
    {
        decimal_skip_interval_size = ( float( global_quota_count + global_supporting_vote_count_that_exceeds_quota - 1.0 ) / float( global_quota_count ) ) ;
    } else
    {
        log_out << "[error, bug has been introduced into code, quota count is zero]" ;
        std::cout << "[Error: Bug has been introduced into code, quota count is zero.]" << std::endl ;
        exit( EXIT_FAILURE ) ;
    }
    if ( global_logging_info == global_true ) { log_out << "[decimal skip interval size " << convert_float_to_text( decimal_skip_interval_size ) << "]" << std::endl ; } ;


// -----------------------------------------------
//  Begin a loop that handles each ballot group.

    global_pointer_to_voteinfo_number = 1 ;
    for ( global_ballot_group_pointer = 1 ; global_ballot_group_pointer <= global_total_count_of_ballot_groups ; global_ballot_group_pointer ++ )
    {


// -----------------------------------------------
//  Get the number of remaining votes in the
//  current ballot group.

        remaining_ballot_count_for_current_ballot_group = global_ballot_count_remaining_for_ballot_group[ global_ballot_group_pointer ] ;


// -----------------------------------------------
//  If the ballot count for this ballot group has
//  already reached zero votes, point to the next
//  ballot group and restart the loop.

        if ( remaining_ballot_count_for_current_ballot_group == 0 )
        {
            point_to_next_ballot_group( ) ;
            continue ;
        }


// -----------------------------------------------
//  If this ballot group only has one top-ranked
//  candidate, get this candidate number.
//  Otherwise put a zero into this variable.

        top_ranked_candidate_if_only_one_else_zero = global_top_ranked_candidate_for_ballot_group[ global_ballot_group_pointer ] ;


// -----------------------------------------------
//  If this ballot group has multiple top-ranked
//  candidates, identify them.  In all cases,
//  count how many candidates are top-ranked on
//  these identical ballots.  Also point to the
//  next ballot group.

        if ( top_ranked_candidate_if_only_one_else_zero == 0 )
        {
            global_ballot_info_repeat_count = get_candidate_ranks_from_one_ballot_group( ) ;
            identify_top_ranked_candidates( ) ;
        } else
        {
        	global_count_of_top_ranked_remaining_candidates = 1 ;
            point_to_next_ballot_group( ) ;
        }


// -----------------------------------------------
//  If this ballot group only has one top-ranked
//  candidate, but it's not the just-elected
//  candidate, repeat the loop.

        if ( ( global_count_of_top_ranked_remaining_candidates == 1 ) && ( top_ranked_candidate_if_only_one_else_zero != global_candidate_just_elected ) )
        {
            continue ;
        }


// -----------------------------------------------
//  If there are multiple top-ranked candidates,
//  but none of them are the just-elected
//  candidate, repeat the loop.
//  Reminder:  The true-or-false list was updated
//  only if there were multiple top-ranked
//  candidates.

        if ( global_count_of_top_ranked_remaining_candidates > 1 )
        {
            if ( global_true_or_false_is_top_ranked_candidate[ global_candidate_just_elected ] != global_true )
            {
                continue ;
            }
        }


// -----------------------------------------------
//  Update the residual vote transfer count with
//  the addition of the remaining ballot count
//  from the current ballot group.

        decimal_residual_vote_transfer_count += float( remaining_ballot_count_for_current_ballot_group ) / float( global_count_of_top_ranked_remaining_candidates ) ;


// -----------------------------------------------
//  Calculate how many ballots, if any, in this
//  ballot group are reduced to zero influence.
//  Then reduce the residual vote transfer count
//  by that number of ballots.
//
//  Do not allow the group's ballot count to
//  increase, or to become less than zero.
//
//  At the end of this function's adjustment
//  process the number of ballots getting zero
//  influence will equal the number of votes by
//  which the winning-candidate's transfer count
//  exceeded the quota count.
//
//  This calculation does not give any ballot
//  partial influence -- between full influence
//  and zero influence -- because many laws
//  require allocating ballots in "whole" numbers.

        number_of_ballots_getting_zero_influence = 1 + int( decimal_residual_vote_transfer_count / decimal_skip_interval_size ) ;
        if ( number_of_ballots_getting_zero_influence < 0 )
        {
        	number_of_ballots_getting_zero_influence = 0 ;
        } else if ( number_of_ballots_getting_zero_influence > remaining_ballot_count_for_current_ballot_group )
        {
        	number_of_ballots_getting_zero_influence = remaining_ballot_count_for_current_ballot_group ;
        }
        decimal_residual_vote_transfer_count -= number_of_ballots_getting_zero_influence ;
        total_number_of_ballots_that_got_zero_influence += number_of_ballots_getting_zero_influence ;


// -----------------------------------------------
//  Update the remaining ballot count for this
//  ballot group.

        global_ballot_count_remaining_for_ballot_group[ global_ballot_group_pointer ] = remaining_ballot_count_for_current_ballot_group - number_of_ballots_getting_zero_influence ;


// -----------------------------------------------
//  Log this ballot count adjustment.

        if ( global_logging_info == global_true ) { log_out << "[group " << global_ballot_group_pointer << ", had " << convert_integer_to_text( remaining_ballot_count_for_current_ballot_group ) << " votes, reduced by " << number_of_ballots_getting_zero_influence << ", now " << convert_integer_to_text( global_ballot_count_remaining_for_ballot_group[ global_ballot_group_pointer ] ) << ", overall residual " << convert_float_to_text( decimal_residual_vote_transfer_count ) << "]" << std::endl ; } ;


// -----------------------------------------------
//  Repeat the loop that accesses all the ballot
//  info, one ballot group at a time.

    }


// -----------------------------------------------
//  If the number of ballots that got zero
//  influence is significantly fewer than the
//  quota count then there is a software bug.

    if ( global_logging_info == global_true ) { log_out << "[" << total_number_of_ballots_that_got_zero_influence << " ballots got zero influence, which must equal or exceed the quota count, which is " << global_quota_count << "]" << std::endl ; } ;
    if ( total_number_of_ballots_that_got_zero_influence < ( global_quota_count - 1 ) )
    {
        log_out << "[error, bug has been introduced into code, " << total_number_of_ballots_that_got_zero_influence << " ballots got zero influence, but that is less than the quota count, which is " << global_quota_count << "]" ;
        std::cout << "[Error: Bug has been introduced into code, " << total_number_of_ballots_that_got_zero_influence << " ballots got zero influence, but that is less than the quota count, which is " << global_quota_count << ".]" << std::endl ;
        exit( EXIT_FAILURE ) ;
    }


// -----------------------------------------------
//  End of function adjust_for_quota_excess.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//        method_rcipe_stv
//
//  Does the calculations that identify which
//  candidates win the seats according to the
//  RCIPE STV method.  If there is just one seat,
//  this is equivalent to the RCIPE method, which
//  modifies the instant-runoff voting (IRV)
//  method.
//
//  If pairwise losing candidates are not
//  eliminated, this code implements the methods
//  known as the "single transferable vote" (STV)
//  (when multiple seats need to be filled) and
//  "instant runoff voting" (IRV) (when there can
//  be only one winner).

void method_rcipe_stv( )
{

    int number_of_seats_filled ;
    int candidate_number ;
    int first_candidate_number ;
    int second_candidate_number ;
    int pair_counter ;
    int pairwise_loser ;
    int pointer_to_list_of_tied_candidates ;
    int preference_level ;
    int pattern_number_pointer ;
    int remaining_pattern_number ;
    int count_of_candidates_at_top_preference_level ;
    int ballot_count_for_shared_preference_level ;
    int remaining_ballot_count_for_current_ballot_group ;
    int pointer_to_list_of_candidates_with_highest_transfer_count ;
    int pointer_to_list_of_candidates_with_lowest_transfer_count ;
    int candidate_number_with_highest_vote_transfer_count ;
    int candidate_number_with_lowest_vote_transfer_count ;
    int number_of_candidates_with_highest_vote_transfer_count ;
    int number_of_candidates_with_lowest_vote_transfer_count ;
    int highest_vote_transfer_count ;
    int lowest_vote_transfer_count ;
    int pattern_number ;
    int count_of_tied_candidates ;

    if ( global_logging_info == global_true ) { log_out << std::endl << "[calculating winner or winners for RCIPE or IRV or RCIPE STV or STV]" << std::endl ; } ;


// -----------------------------------------------
//  Set a flag that will be used during the first
//  counting cycle to initialize each ballot group
//  to have full influence.

    global_need_to_initialize_group_ballot_count = global_true ;


// -----------------------------------------------
//  Begin the loop that handles each counting
//  cycle.  The loop exits when all the available
//  seats have been filled, or a tie is
//  encountered.

    for ( global_counting_cycle_number = 1 ; global_counting_cycle_number <= global_number_of_candidates + 1 ; global_counting_cycle_number ++ )
    {
        if ( global_logging_info == global_true ) { log_out << std::endl << "[starting counting cycle number " << global_counting_cycle_number << "]" << std::endl ; } ;


// -----------------------------------------------
//  Indicate that, so far, a candidate has not
//  been elected during this counting cycle.

        global_candidate_just_elected = 0 ;


// -----------------------------------------------
//  Count (and log) which candidates have won,
//  been eliminated, or are still being
//  considered.  Also ensure the flags that
//  indicate availability (to get elected) do not
//  include candidates who have been elected or
//  eliminated.

        number_of_seats_filled = 0 ;
        global_number_of_remaining_candidates = 0 ;
        for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
        {
            if ( global_true_or_false_winner_candidate[ candidate_number ] == global_true )
            {
                global_true_or_false_available_candidate[ candidate_number ] = global_false ;
                number_of_seats_filled ++ ;
                if ( global_logging_info == global_true ) { log_out << "[candidate " << candidate_number << " elected !!!!]" ; } ;
            } else if ( global_true_or_false_eliminated_candidate[ candidate_number ] == global_true )
            {
                global_true_or_false_available_candidate[ candidate_number ] = global_false ;
                if ( global_logging_info == global_true ) { log_out << "[candidate " << candidate_number << " eliminated ----]" ; } ;
            } else if ( global_true_or_false_available_candidate[ candidate_number ] == global_true )
            {
                global_number_of_remaining_candidates ++ ;
                if ( global_logging_info == global_true ) { log_out << "[candidate " << candidate_number << " available]" ; } ;
            } else
            {
                log_out << "[error, bug has been introduced into code, candidate status is invalid]" ;
                std::cout << "[Error: Bug has been introduced into code, candidate status is invalid.]" << std::endl ;
                exit( EXIT_FAILURE ) ;
            }
        }
        if ( global_logging_info == global_true ) { log_out << std::endl ; } ;
        global_number_of_seats_still_available = global_number_of_seats_to_fill - number_of_seats_filled ;


// -----------------------------------------------
//  If all the seats have been filled, return from
//  this function.

        if ( global_number_of_seats_still_available == 0 )
        {
            if ( global_logging_info == global_true ) { log_out << "[all " << global_number_of_seats_to_fill << " seats have been filled]" << std::endl ; } ;
            return ;
        }


// -----------------------------------------------
//  If there are no candidates available to fill
//  any more seats, indicate a tie, and return
//  from this function.

        if ( global_number_of_remaining_candidates == 0 )
        {
            if ( global_logging_info == global_true ) { log_out << "[all the candidates have been eliminated or elected without filling all the seats, so there is a tie among the eliminated candidates that must be resolved]" << std::endl ; } ;
            return ;
        }


// -----------------------------------------------
//  Indicate how many seats are still available.

        if ( global_logging_info == global_true ) { log_out << "[" << global_number_of_seats_still_available << " seats still available]" << std::endl ; } ;


// -----------------------------------------------
//  If there is only one not-yet-eliminated
//  candidate and one still-available seat,
//  identify the candidate as a winner and restart
//  the counting cycle.

        if ( ( global_number_of_seats_still_available == 1 ) && ( global_number_of_remaining_candidates == 1 ) )
        {
            for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
            {
                if ( global_true_or_false_available_candidate[ candidate_number ] == global_true )
                {
                    global_candidate_just_elected = candidate_number ;
                	break ;
                }
            }
            global_true_or_false_winner_candidate[ global_candidate_just_elected ] = global_true ;
            global_true_or_false_available_candidate[ global_candidate_just_elected ] = global_false ;
            put_next_result_info_number( global_voteinfo_code_for_winner_next_seat ) ;
            put_next_result_info_number( global_candidate_just_elected ) ;
            if ( global_logging_info == global_true ) { log_out << std::endl << "[there is one seat available and one remaining candidate, so candidate " << global_candidate_just_elected << " wins the last seat]" << std::endl ; } ;
            continue ;
        }


// -----------------------------------------------
//  Reset the vote transfer counts of all the
//  candidates to start at zero.

        for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
        {
            global_vote_transfer_count_for_candidate[ candidate_number ] = 0 ;
        }


// -----------------------------------------------
//  Initialize the counter for the total vote
//  count.  The total vote count can be different
//  in each counting cycle because some ballots
//  switch to zero influence after helping to
//  elect a candidate.

        global_current_total_vote_count = 0 ;


// -----------------------------------------------
//  Begin a loop that handles each group of
//  identical ballots, one ballot group at a time.

        global_count_of_unique_pattern_numbers = 0 ;
        global_pointer_to_voteinfo_number = 1 ;
        for ( global_ballot_group_pointer = 1 ; global_ballot_group_pointer <= global_total_count_of_ballot_groups ; global_ballot_group_pointer ++ )
        {


// -----------------------------------------------
//  For the current ballot group, identify which
//  remaining (not-yet-eliminated and
//  not-yet-elected) candidate is ranked highest.
//  Allow for two or more candidates to be
//  highest-ranked.  Also get the ballot count
//  that indicates how many ballots are in this
//  ballot group (of identical ballots), and get
//  this ballot group's remaining vote count.

            global_ballot_info_repeat_count = get_candidate_ranks_from_one_ballot_group( ) ;


// -----------------------------------------------
//  Add this ballot group's remaining vote count
//  to the sum of all the remaining vote counts.

            global_current_total_vote_count += global_ballot_count_remaining_for_ballot_group[ global_ballot_group_pointer ] ;


// -----------------------------------------------
//  If the end of the ballot groups has been
//  reached, exit the loop that handles each
//  ballot group.

            if ( global_ballot_info_repeat_count < 1 )
            {
                break ;
            }


// -----------------------------------------------
//  Add the full-influence (remaining) ballots in
//  the current ballot group to a counter that
//  applies to all the ballots that have the same
//  available (remaining) candidates ranked above
//  all the other available candidates.
//
//  For example, if candidates A and B have been
//  elected or eliminated, and a ballot group
//  ranks candidate A highest and ranks candidates
//  B, C, and D at the next-highest level, and
//  another ballot group ranks candidate B highest
//  and ranks candidates A, C, and D at the
//  next-highest level, then these two ballot
//  groups are counted together, along with any
//  other ballot groups that rank candidates C and
//  D as the highest-ranked remaining candidates.

            add_current_ballot_group_votes_to_vote_transfer_counts( ) ;


// -----------------------------------------------
//  Repeat the loop that gets information from
//  each ballot group.

        }


// -----------------------------------------------
//  The group ballot counts have been initialized
//  to full influence, so ensure that this
//  initialization is not done again.

        global_need_to_initialize_group_ballot_count = global_false ;


// -----------------------------------------------
//  Distribute to the remaining candidates the
//  ballot counts in the lists that group together
//  the same (equivalent) top-ranking pattern.
//  The resulting counts specify how many ballots
//  transfer to each remaining candidate.  Also
//  calculate the total vote count.

        calculate_transfer_count_for_each_candidate( ) ;


// -----------------------------------------------
//  If only one seat is to be filled, calculate
//  the quota count as the majority, which equals
//  one plus half the ballots that mark any
//  preference for the remaining candidates.

        if ( global_number_of_seats_to_fill == 1 )
        {
            global_quota_count = 1 + int( float( global_current_total_vote_count ) / 2.0 ) ;
            global_text_quota_type = global_text_quota_type_majority ;


// -----------------------------------------------
//  If the Droop quota is requested, calculate it.
//  It equals one plus the total ballot count
//  divided by the quantity one plus the seat
//  count.

        } else if ( global_true_or_false_request_quota_droop == global_true )
        {
            global_quota_count = 1 + int( float( global_current_total_vote_count ) / float( 1 + global_number_of_seats_to_fill ) ) ;
            global_text_quota_type = global_text_quota_type_droop ;


// -----------------------------------------------
//  When multiple seats are being filled, and the
//  Droop quota has not been requested, calculate
//  the Hare quota for use as the quota count.
//  The Hare quota count equals one plus the total
//  ballot count divided by the seat count.
//
//  The Hare quota is the default because it
//  produces results that are more proportional.
//  The Droop quota produces results that are less
//  proportional, yet is useful when political
//  parties are involved (to avoid giving a seat
//  to a very small party), and when ballots are
//  counted without the use of a computer.
//
//  Reminder:  The quota count can change between
//  counting cycles for two reasons.  Some ballots
//  might not rank any of the remaining candidates
//  higher than the other remaining candidates.
//  And the total ballot count changes after a
//  candidate is elected because some ballots that
//  supported that winning candidate must be
//  reduced to zero influence so that the same
//  voters do not also influence which candidates
//  win the other seats.

        } else
        {
            global_quota_count = 1 + int( float( global_current_total_vote_count ) / float( global_number_of_seats_to_fill ) ) ;
            global_text_quota_type = global_text_quota_type_hare ;
        }


// -----------------------------------------------
//  Display the current total vote count and the
//  calculated quota count.

        if ( global_logging_info == global_true ) { log_out << "[total vote count is " << global_current_total_vote_count << ", quota count is " << global_quota_count << ", quota type is " << global_text_quota_type << "]" << std::endl ; } ;


// -----------------------------------------------
//  Identify which candidate has the highest
//  number of ballots transferred to that
//  candidate.  Allow for the possibility of a
//  tie.

        highest_vote_transfer_count = -1 ;
        pointer_to_list_of_candidates_with_highest_transfer_count = 0 ;
        for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
        {
            if ( global_true_or_false_available_candidate[ candidate_number ] == global_true )
            {
                if ( global_logging_info == global_true ) { log_out << "[candidate " << candidate_number << " transfer count is " << global_vote_transfer_count_for_candidate[ candidate_number ] << "]" << std::endl ; } ;
                if ( ( pointer_to_list_of_candidates_with_highest_transfer_count == 0 ) || ( global_vote_transfer_count_for_candidate[ candidate_number ] > highest_vote_transfer_count ) )
                {
                    highest_vote_transfer_count = global_vote_transfer_count_for_candidate[ candidate_number ] ;
                    pointer_to_list_of_candidates_with_highest_transfer_count = 1 ;
                    global_list_of_candidates_with_highest_vote_transfer_count[ 1 ] = candidate_number ;
                } else if ( global_vote_transfer_count_for_candidate[ candidate_number ] == highest_vote_transfer_count )
                {
                    pointer_to_list_of_candidates_with_highest_transfer_count ++ ;
                    global_list_of_candidates_with_highest_vote_transfer_count[ pointer_to_list_of_candidates_with_highest_transfer_count ] = candidate_number ;
                }
            }
        }
        number_of_candidates_with_highest_vote_transfer_count = pointer_to_list_of_candidates_with_highest_transfer_count ;


// -----------------------------------------------
//  If the highest vote transfer count is zero (or
//  negative), or if zero candidates have the
//  highest vote transfer count, then there is a
//  tie.

        if ( ( highest_vote_transfer_count <= 0 ) || ( number_of_candidates_with_highest_vote_transfer_count < 1 ) )
        {
            log_out << "[error, zero candidates have the highest vote transfer count, which means there is a tie that must be resolved]" << std::endl ;
            std::cout << "[Error: Zero candidates have the highest vote transfer count, which means there is a tie that must be resolved.]" << std::endl ;
            exit( EXIT_FAILURE ) ;
        }


// -----------------------------------------------
//  Show which candidates have the highest
//  transfer count.

        if ( global_logging_info == global_true ) { log_out << "[" << number_of_candidates_with_highest_vote_transfer_count << " candidates have the same highest vote transfer count of " << highest_vote_transfer_count << "]" << std::endl ; } ;
       for ( pointer_to_list_of_candidates_with_highest_transfer_count = 1 ; pointer_to_list_of_candidates_with_highest_transfer_count <= number_of_candidates_with_highest_vote_transfer_count ; pointer_to_list_of_candidates_with_highest_transfer_count ++ )
        {
            if ( global_logging_info == global_true ) { log_out << "[candidate " << global_list_of_candidates_with_highest_vote_transfer_count[ pointer_to_list_of_candidates_with_highest_transfer_count ] << " has highest vote transfer count]" << std::endl ; } ;
        }


// -----------------------------------------------
//  Calculate how many ballots beyond the quota
//  count support the candidate (or candidates)
//  who has (or have) the highest vote transfer
//  count.

        global_supporting_vote_count_that_exceeds_quota = highest_vote_transfer_count - global_quota_count ;


// -----------------------------------------------
//  If one, and only one, candidate has a vote
//  transfer count that meets or exceeds the
//  quota, elect this candidate.

        if ( ( number_of_candidates_with_highest_vote_transfer_count == 1 ) && ( global_supporting_vote_count_that_exceeds_quota >= 0 ) )
        {
            global_supporting_votes_for_elected_candidate = highest_vote_transfer_count ;
            global_candidate_just_elected = global_list_of_candidates_with_highest_vote_transfer_count[ 1 ] ;
            global_true_or_false_winner_candidate[ global_candidate_just_elected ] = global_true ;
            global_true_or_false_available_candidate[ global_candidate_just_elected ] = global_false ;
            put_next_result_info_number( global_voteinfo_code_for_winner_next_seat ) ;
            put_next_result_info_number( global_candidate_just_elected ) ;
            global_number_of_seats_still_available -- ;
            if ( global_logging_info == global_true ) { log_out << std::endl << "[candidate " << global_candidate_just_elected << " won a seat by reaching the quota count]" << std::endl ; } ;
        }


// -----------------------------------------------
//  If all the seats have been filled, repeat the
//  counting cycle loop because vote transfers are
//  not needed.

        if ( global_number_of_seats_still_available == 0 )
        {
            if ( global_logging_info == global_true ) { log_out << "[all " << global_number_of_seats_to_fill << " seats have been filled]" << std::endl ; } ;
            continue ;
        }


// -----------------------------------------------
//  If more than one candidate has the same
//  highest vote transfer count, and if the count
//  meets or exceeds the quota count, eliminate
//  from the tie any pairwise losing candidates
//  within the tie. Allow for multiple pairwise
//  losing candidates among the tied candidates.

        if ( ( number_of_candidates_with_highest_vote_transfer_count > 1 ) && ( global_supporting_vote_count_that_exceeds_quota >= 0 ) )
        {
            global_supporting_votes_for_elected_candidate = highest_vote_transfer_count ;
            for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
            {
                global_true_or_false_pairwise_consider_candidate[ candidate_number ] = global_false ;
            }
            for ( pointer_to_list_of_candidates_with_highest_transfer_count = 1 ; pointer_to_list_of_candidates_with_highest_transfer_count <= number_of_candidates_with_highest_vote_transfer_count ; pointer_to_list_of_candidates_with_highest_transfer_count ++ )
            {
                candidate_number = global_list_of_candidates_with_highest_vote_transfer_count[ pointer_to_list_of_candidates_with_highest_transfer_count ] ;
                global_true_or_false_pairwise_consider_candidate[ candidate_number ] = global_true ;
            }
            count_of_tied_candidates = number_of_candidates_with_highest_vote_transfer_count ;
            while ( count_of_tied_candidates > 1 )
            {
                reset_tally_table( ) ;
                if ( global_pair_counter_maximum == 0 )
                {
                    log_out << "[error, the number of candidates being pairwise counted to resolve a tie exceeds the available storage space, so need to increase storage space or add one ballot that resolves the tie]" ;
                    std::cout << "[Error: The number of candidates being pairwise counted to resolve a tie exceeds the available storage space, so need to increase storage space or add one ballot that resolves the tie.]" << std::endl ;
                    exit( EXIT_FAILURE ) ;
                } else
                {
                    if ( global_logging_info == global_true ) { log_out << "[looking for pairwise losing candidate within a tie]" << std::endl ; } ;
                    fill_pairwise_tally_table( ) ;
                    pairwise_loser = check_for_pairwise_losing_candidate( ) ;
                    if ( pairwise_loser > 0 )
                    {
                        count_of_tied_candidates -- ;
                        if ( global_logging_info == global_true ) { log_out << "[candidate " << pairwise_loser << " is a pairwise losing candidate among the tied candidates, so it is removed from the list of tied candidates]" << std::endl ; } ;
                        global_true_or_false_pairwise_consider_candidate[ pairwise_loser ] = global_false ;
                    } else
                    {
                        if ( global_logging_info == global_true ) { log_out << "[there are no more pairwise losing candidates among the tied candidates, so the tie cannot be resolved any farther]" << std::endl ; } ;
                        break ;
                    }
                }
            }


// -----------------------------------------------
//  If the tie has been reduced to just one
//  candidate, that candidate wins a seat.

            if ( count_of_tied_candidates == 1 )
            {
                for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
                {
                    if ( global_true_or_false_pairwise_consider_candidate[ candidate_number ] == global_true )
                    {
                        global_candidate_just_elected = candidate_number ;
                        break ;
                    }
                }
                global_true_or_false_winner_candidate[ global_candidate_just_elected ] = global_true ;
                global_true_or_false_available_candidate[ global_candidate_just_elected ] = global_false ;
                put_next_result_info_number( global_voteinfo_code_for_winner_next_seat ) ;
                put_next_result_info_number( global_candidate_just_elected ) ;
                if ( global_logging_info == global_true ) { log_out << std::endl << "[candidate " << global_candidate_just_elected << " is elected as the winner of this tie among the candidates with the highest vote transfer count]" << std::endl ; } ;


// -----------------------------------------------
//  The tie among the candidates who have the
//  same highest vote transfer count cannot be
//  resolved, so indicate this tie and which
//  candidates are tied, then return from this
//  function.

            } else
            {
                put_next_result_info_number( global_voteinfo_code_for_begin_tied_for_next_seat ) ;
                if ( global_logging_info == global_true ) { log_out << "[there is a tie among " << global_number_of_remaining_candidates << " candidates who have the same highest pairwise support count that exceeds the quota count]" << std::endl ; } ;
                for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
                {
                    if ( global_true_or_false_available_candidate[ candidate_number ] == global_true )
                    {
                        put_next_result_info_number( candidate_number ) ;
                        if ( global_logging_info == global_true ) { log_out << "[candidate " << candidate_number << " is one of the candidates tied with the highest vote transfer count]" << std::endl ; } ;
                    }
                }
                put_next_result_info_number( global_voteinfo_code_for_end_tied_for_next_seat ) ;
                return ;
            }


// -----------------------------------------------
//  Done handling one or more candidates meeting
//  or exceeding the quota count.

        }


// -----------------------------------------------
//  If all the seats have been filled, return from
//  this function.

        if ( global_number_of_seats_still_available == 0 )
        {
            if ( global_logging_info == global_true ) { log_out << "[all " << number_of_seats_filled << " seats have been filled]" << std::endl ; } ;
            return ;
        }


// -----------------------------------------------
//  If none of the candidates met or exceeded the
//  quota count, log this fact.

        if ( global_supporting_vote_count_that_exceeds_quota < 0 )
        {
            if ( global_logging_info == global_true ) { log_out << "[none of the candidates reached the quota]" << std::endl ; } ;
        }


// -----------------------------------------------
//  If none of the candidates met or exceeded the
//  quota count, and if the number of remaining
//  candidates equals the number of seats still
//  available, elect the candidate or candidates
//  who have the highest transfer count, even
//  though the quota was not reached.

        if ( ( global_supporting_vote_count_that_exceeds_quota < 0 ) && ( global_number_of_remaining_candidates == global_number_of_seats_still_available ) )
        {
            if ( number_of_candidates_with_highest_vote_transfer_count == 1 )
            {
                global_candidate_just_elected = global_list_of_candidates_with_highest_vote_transfer_count[ 1 ] ;
                put_next_result_info_number( global_voteinfo_code_for_winner_next_seat ) ;
                put_next_result_info_number( global_candidate_just_elected ) ;
                global_number_of_seats_still_available -- ;
                if ( global_logging_info == global_true ) { log_out << "[candidate " << global_candidate_just_elected << " wins the next seat because the number of remaining candidates equals the number of remaining seats and this candidate has the highest vote transfer count]" ; } ;
            } else
            {
                put_next_result_info_number( global_voteinfo_code_for_begin_tied_for_next_seat ) ;
                if ( global_logging_info == global_true ) { log_out << "[there are " << number_of_candidates_with_highest_vote_transfer_count << " remaining candidates tied with the highest vote transfer count, and they all win a seat because the number of remaining candidates equals the number of remaining seats]" ; } ;
                for ( pointer_to_list_of_candidates_with_highest_transfer_count = 1 ; pointer_to_list_of_candidates_with_highest_transfer_count <= number_of_candidates_with_highest_vote_transfer_count ; pointer_to_list_of_candidates_with_highest_transfer_count ++ )
                {
                    global_candidate_just_elected = global_list_of_candidates_with_highest_vote_transfer_count[ pointer_to_list_of_candidates_with_highest_transfer_count ] ;
                    global_true_or_false_winner_candidate[ global_candidate_just_elected ] = global_true ;
                    global_true_or_false_available_candidate[ global_candidate_just_elected ] = global_false ;
                    put_next_result_info_number( global_candidate_just_elected ) ;
                    global_number_of_seats_still_available -- ;
                    if ( global_logging_info == global_true ) { log_out << "[candidate " << global_candidate_just_elected << " wins a seat because the number of remaining candidates equals the number of remaining seats]" ; } ;
                }
                put_next_result_info_number( global_voteinfo_code_for_end_tied_for_next_seat ) ;
            }
            continue ;
        }


// -----------------------------------------------
//  If a counting cycle ended with a candidate
//  getting elected, reduce to zero influence most
//  of the supporting ballots that caused this
//  candidate to win.  Retain influence for some
//  ballots to the extent that the transfer count
//  exceeded the quota count.  Choose which
//  ballots are in each category in a way that is
//  extremely unlikely to yield a different
//  election result if the ballot info is supplied
//  in a different sequence.  Then repeat the
//  counting loop.

        if ( global_candidate_just_elected > 0 )
        {
            adjust_for_quota_excess( ) ;
            continue ;
        }


// -----------------------------------------------
//  If requested, skip over the elimination of
//  pairwise losing candidates.

        if ( global_true_or_false_request_no_pairwise_loser_elimination == global_true )
        {
            if ( global_logging_info == global_true ) { log_out << "[by request, pairwise losing candidates are not eliminated]" << std::endl ; } ;


// -----------------------------------------------
//  None of the candidates were elected, so look
//  for a pairwise losing candidate, and repeat
//  the counting-cycle loop.  If there is one,
//  eliminate that candidate.  A pairwise losing
//  candidate is a candidate that would lose every
//  one-on-one match against every other remaining
//  (not-yet-elected and not-yet-eliminated)
//  candidate.  During this pairwise counting the
//  ballots that have zero influence do not
//  contribute any votes to either side of the
//  one-on-one matches.

        } else
        {
            for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
            {
                if ( global_true_or_false_available_candidate[ candidate_number ] == global_true )
                {
                    global_true_or_false_pairwise_consider_candidate[ candidate_number ] = global_true ;
                } else
                {
                    global_true_or_false_pairwise_consider_candidate[ candidate_number ] = global_false ;
                }
            }
            reset_tally_table( ) ;
            if ( global_pair_counter_maximum == 0 )
            {
                log_out << "[error, the number of candidates being pairwise counted exceeds the available storage space, so need to increase storage space or explicitly eliminate one or more clearly unpopular candidates]" ;
                std::cout << "[Error: The number of candidates being pairwise counted exceeds the available storage space, so need to increase storage space or explicitly eliminate one or more clearly unpopular candidates.]" << std::endl ;
                exit( EXIT_FAILURE ) ;
            } else
            {
                if ( global_logging_info == global_true ) { log_out << "[looking for pairwise losing candidate to eliminate]" << std::endl ; } ;
                fill_pairwise_tally_table( ) ;
                pairwise_loser = check_for_pairwise_losing_candidate( ) ;
                if ( pairwise_loser > 0 )
                {
                    global_true_or_false_eliminated_candidate[ pairwise_loser ] = global_true ;
                    global_true_or_false_available_candidate[ pairwise_loser ] = global_false ;
                    put_next_result_info_number( global_voteinfo_code_for_pairwise_losing_candidate ) ;
                    put_next_result_info_number( candidate_number ) ;
                    global_number_of_seats_still_available -- ;
                    if ( global_logging_info == global_true ) { log_out << std::endl << "[eliminating candidate " << pairwise_loser << " because is pairwise losing candidate]" << std::endl ; } ;
                    continue ;
                }
            }
        }


// -----------------------------------------------
//  A pairwise losing candidate was not found, or
//  there was a request to skip that check, so
//  identify which of the remaining candidates has
//  the lowest number of ballots transferred to
//  that candidate.  Allow for the possibility of
//  a tie.

        if ( global_logging_info == global_true ) { log_out << "[looking for candidate with lowest vote transfer count]" << std::endl ; } ;
        lowest_vote_transfer_count = -1 ;
        pointer_to_list_of_candidates_with_lowest_transfer_count = 0 ;
        for ( candidate_number = 1 ; candidate_number <= global_number_of_candidates ; candidate_number ++ )
        {
            if ( global_true_or_false_available_candidate[ candidate_number ] == global_true )
            {
//                if ( global_logging_info == global_true ) { log_out << "[candidate " << candidate_number << " transfer count is " << global_vote_transfer_count_for_candidate[ candidate_number ] << "]" << std::endl ; } ;
                if ( ( pointer_to_list_of_candidates_with_lowest_transfer_count == 0 ) || ( global_vote_transfer_count_for_candidate[ candidate_number ] < lowest_vote_transfer_count ) )
                {
                    lowest_vote_transfer_count = global_vote_transfer_count_for_candidate[ candidate_number ] ;
                    pointer_to_list_of_candidates_with_lowest_transfer_count = 1 ;
                    global_list_of_candidates_with_lowest_vote_transfer_count[ pointer_to_list_of_candidates_with_lowest_transfer_count ] = candidate_number ;
                } else if ( global_vote_transfer_count_for_candidate[ candidate_number ] == lowest_vote_transfer_count )
                {
                    pointer_to_list_of_candidates_with_lowest_transfer_count ++ ;
                    global_list_of_candidates_with_lowest_vote_transfer_count[ pointer_to_list_of_candidates_with_lowest_transfer_count ] = candidate_number ;
                }
            }
        }
        number_of_candidates_with_lowest_vote_transfer_count = pointer_to_list_of_candidates_with_lowest_transfer_count ;
        if ( global_logging_info == global_true ) { log_out << "[" << number_of_candidates_with_lowest_vote_transfer_count << " candidates have the same lowest vote transfer count]" << std::endl ; } ;
        for ( pointer_to_list_of_candidates_with_lowest_transfer_count = 1 ; pointer_to_list_of_candidates_with_lowest_transfer_count <= number_of_candidates_with_lowest_vote_transfer_count ; pointer_to_list_of_candidates_with_lowest_transfer_count ++ )
        {
        	candidate_number = global_list_of_candidates_with_lowest_vote_transfer_count[ pointer_to_list_of_candidates_with_lowest_transfer_count ] ;
            if ( global_logging_info == global_true ) { log_out << "[candidate " << candidate_number << " has lowest vote transfer count of " << global_vote_transfer_count_for_candidate[ candidate_number ] << "]" << std::endl ; } ;
        }


// -----------------------------------------------
//  If the lowest vote transfer count is less than
//  one, that is an error.

        if ( number_of_candidates_with_lowest_vote_transfer_count < 1 )
        {
            log_out << "[error, zero candidates have the lowest vote transfer count]" << std::endl ;
            std::cout << "[Error: Zero candidates have the lowest vote transfer count.]" << std::endl ;
            exit( EXIT_FAILURE ) ;
        }


// -----------------------------------------------
//  Eliminate the candidate with the lowest
//  top-ranked count, then repeat the main
//  counting loop.

        if ( number_of_candidates_with_lowest_vote_transfer_count == 1 )
        {
            candidate_number = global_list_of_candidates_with_lowest_vote_transfer_count[ 1 ] ;
            global_true_or_false_eliminated_candidate[ candidate_number ] = global_true ;
            global_true_or_false_available_candidate[ candidate_number ] = global_false ;
            put_next_result_info_number( global_voteinfo_code_for_eliminated_candidate ) ;
            put_next_result_info_number( candidate_number ) ;
            if ( global_logging_info == global_true ) { log_out << "[eliminating candidate " << candidate_number << " because has lowest vote transfer count of " << global_vote_transfer_count_for_candidate[ candidate_number ] << "]" << std::endl ; } ;
            continue ;
        }


// -----------------------------------------------
//  There is more than one candidate with the
//  lowest vote transfer count, so eliminate all
//  those candidates, then repeat the main loop.

        if ( global_logging_info == global_true ) { log_out << "[eliminating all the candidates who have the same lowest vote transfer count]" << std::endl ; } ;
        for ( pointer_to_list_of_tied_candidates = 1 ; pointer_to_list_of_tied_candidates <= number_of_candidates_with_lowest_vote_transfer_count ; pointer_to_list_of_tied_candidates ++ )
        {
            candidate_number = global_list_of_candidates_with_lowest_vote_transfer_count[ pointer_to_list_of_tied_candidates ] ;
            global_true_or_false_eliminated_candidate[ candidate_number ] = global_true ;
            global_true_or_false_available_candidate[ candidate_number ] = global_false ;
            put_next_result_info_number( global_voteinfo_code_for_eliminated_candidate ) ;
            put_next_result_info_number( candidate_number ) ;
            if ( global_logging_info == global_true ) { log_out << "[eliminating candidate " << candidate_number << " who is tied with the lowest vote transfer count]" << std::endl ; } ;
        }


// -----------------------------------------------
//  Repeat the loop that handles one counting
//  cycle.  If the counting cycles did not either
//  fill all the seats or encounter a tie, then
//  there was an error.

    }
    log_out << "[error, reached end of all counting cycles without exiting the counting-cycles loop gracefully]" ;
    std::cout << "[Error: Reached end of all counting cycles without exiting the counting-cycles loop gracefully.]" << std::endl ;
    exit( EXIT_FAILURE ) ;


// -----------------------------------------------
//  End of function method_rcipe_stv.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//  Execution starts here.
//  Do initialization, then do the processing.

int main() {


    int current_result_info_number ;
    std::string newline_or_space ;


// -----------------------------------------------
//  Initialization.

    do_main_initialization( ) ;


// -----------------------------------------------
//  Read input data from standard input (which
//  typically is a file).

    read_data( ) ;


// -----------------------------------------------
//  Do the election calculations.

    method_rcipe_stv( ) ;


// -----------------------------------------------
//  Write the results to standard output, which
//  typically is a file.

    put_next_result_info_number( global_voteinfo_code_for_end_of_all_cases ) ;
    global_length_of_result_info_list = global_pointer_to_output_results + 1 ;
    if ( global_logging_info == global_true ) { log_out << std::endl << "[all results, length of output list is " << global_length_of_result_info_list << "]" << std::endl ; } ;
    newline_or_space = "" ;
    global_pointer_to_output_results = 0 ;
    current_result_info_number = 0 ;
    while ( ( current_result_info_number != global_voteinfo_code_for_end_of_all_cases ) && ( global_pointer_to_output_results < global_length_of_result_info_list ) && ( global_pointer_to_output_results < global_maximum_output_results_length ) )
    {
        current_result_info_number = global_output_results[ global_pointer_to_output_results ] ;
        global_pointer_to_output_results ++ ;
        if ( global_logging_info == global_true ) { log_out << "[" << current_result_info_number << "]" ; } ;
        std::cout << newline_or_space ;
        if ( current_result_info_number == 0 )
        {
            std::cout << "0" ;
        } else if ( current_result_info_number > 0 )
        {
            std::cout << convert_integer_to_text( current_result_info_number ) ;
        } else
        {
            std::cout << convert_integer_to_text( current_result_info_number ) ;
            if ( newline_or_space == "" )
            {
                newline_or_space = "\n" ;
            }
        }
    }
    std::cout << newline_or_space ;
    std::cout << convert_integer_to_text( global_voteinfo_code_for_end_of_all_cases ) ;
    std::cout << newline_or_space << "0" << std::endl ;
    if ( global_logging_info == global_true ) { log_out << std::endl << "[end output result codes]" << std::endl ; } ;


// -----------------------------------------------
//  End of "main" code.

    if ( global_logging_info == global_true ) { log_out << std::endl << "[results written, all done]" << std::endl ; } ;

}



// -----------------------------------------------
// -----------------------------------------------
//
//  End of all code.
//
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
//  Please report any bugs or feature requests on
//  GitHub, at the CPSolver account, in the
//  VoteFair-ranking-cpp project area.  Thank you!
//
//
// -----------------------------------------------
//
//  SUPPORT
//
//  You can find documentation for this code on
//  GitHub, in the CPSolver account, in the
//  VoteFair-ranking-cpp project area.
//
//
// -----------------------------------------------
//
//  ACKNOWLEDGEMENTS
//
//  Richard Fobes designed the RCIPE and RCIPE STV
//  methods.  Richard Fobes is the author of the
//  books titled "The Creative Problem Solver's
//  Toolbox" and "Ending The Hidden Unfairness In
//  U.S. Elections."
//
//
// -----------------------------------------------
//
//  COPYRIGHT & LICENSE
//
//  (c) Copyright 2022 by Richard Fobes at
//  www.VoteFair.org.  You can redistribute and/or
//  modify this rcipe_stv software under the MIT
//  software license terms as written above.
//  (Also a copy of the license is included in the
//  LICENSE file.)
//
//  Conversion of this code into another
//  programming language is also covered by the
//  above license terms.
//
//  The mathematical algorithms of RCIPE and
//  RCIPE STV are in the public domain.
//
//
// -----------------------------------------------
//
//  End of rcipe_stv.cpp
