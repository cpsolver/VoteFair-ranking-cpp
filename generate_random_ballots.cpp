//  generate_random_ballots.cpp
//
//  This program generates random ballots for use with the
//  votefair_ranking.cpp code.
//  See the ABOUT section for details.
//
//
// -----------------------------------------------
//
//  COPYRIGHT & LICENSE
//
//  (c) Copyright 2021 by Richard Fobes at www.VoteFair.org.
//  You can redistribute and/or modify this VoteFairRanking software
//  under the MIT software license terms that appear
//  in the "license" file.
//
//  Conversion of this code into another programming language
//  is also covered by the above license terms.
//
//
// -----------------------------------------------
//
//  VERSION
//
//  Version 2.1
//
//
// -----------------------------------------------
//
//  USAGE
//
//  This program generates random ballots for use with the
//  votefair_ranking.cpp code.
//
//  The following sample code compiles and then executes
//  this software under a typical Windows environment
//  with the g++ compiler and the mingw32 library already
//  installed.
//
//      path=C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin\
//
//      g++ generate_random_ballots.cpp -o generate_random_ballots
//
//      .\generate_random_ballots > output_generate_random_ballots.txt
//
//
// -----------------------------------------------
//
//  ABOUT
//
//  This software generates random ballots for the
//  votefair_ranking.cpp code and then calculates
//  results for the following three tests:
//
//  * Calculates how often each method yields the
//    same winner as the Condorcet-Kemeny method
//    (which deeply looks into ALL the ballot
//    information).
//
//  * Calculates how often each method yields a
//    different winner if any one of the other
//    non-winning candidates did not enter the
//    race.  Such failures fail the "independence
//    of irrelevant alternatives" (IIA) criterion.
//
//  * Calculates how often each method yields a
//    different winner if a clone candidate enters
//    the race.  Such failures fail the
//    "independence of clones" criterion.
//
//
// -----------------------------------------------
//
//  LICENSE
//
//  This generate_random_ballots software is licensed
//  under the MIT License terms.
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
#include <random>
#include <chrono>


// -----------------------------------------------
//  Change these values as needed to specify the
//  number of ballots, the largest and smallest
//  number of choices, and the number of cases
//  for each choice count.
//  The cases are split among multiple tests, and
//  some cases result in ties that are unusable
//  for comparison, so the case counts do not
//  specify the number of tests.
//  The number of cloned choices does not
//  include the original similar choice.
//  NONE of these values can be zero!

int global_choice_count_minimum = 2 ;
int global_choice_count_maximum = 7 ;
int global_maximum_case_count_per_choice_count = 400 ;
int global_maximum_ballot_number = 11 ;
int global_number_of_clones = 3 ;


// -----------------------------------------------
//  Ensure that the case numbers have the same
//  number of digits.  This number can be changed.
//  Note that some tests use multiple cases.

const int global_minimum_case_id = 100000 ;


// -----------------------------------------------
//  Assign numbers and names to the vote-counting
//  methods, and specify how many methods there
//  are.

int global_number_of_methods = 9 ;
const int global_method_votefair = 1 ;
const int global_method_ipe = 2 ;
const int global_method_rcipe = 3 ;
const int global_method_irvbtr = 4 ;
const int global_method_star = 5 ;
const int global_method_borda = 6 ;
const int global_method_irv = 7 ;
const int global_method_plurality = 8 ;
const int global_method_ple = 9 ;

std::string global_name_for_method_votefair = "VF" ;
std::string global_name_for_method_ipe = "IPE" ;
std::string global_name_for_method_rcipe = "RCIPE" ;
std::string global_name_for_method_irvbtr = "IRV_BTR" ;
std::string global_name_for_method_star = "STAR/NT" ;
std::string global_name_for_method_borda = "Borda/NT" ;
std::string global_name_for_method_irv = "IRV" ;
std::string global_name_for_method_plurality = "PLUR" ;
std::string global_name_for_method_ple = "PLE" ;

std::string global_full_name_for_method_votefair = "Condorcet-Kemeny (VoteFair popularity ranking)" ;
std::string global_full_name_for_method_ipe = "Instant Pairwise Elimination (IPE)" ;
std::string global_full_name_for_method_rcipe = "Ranked Choice Including Pairwise Elimination (RCIPE)" ;
std::string global_full_name_for_method_irvbtr = "IRV with bottom two runoff (IRV_BTR)" ;
std::string global_full_name_for_method_star = "Score Then Automatic Runoff (STAR) with no tactical voting" ;
std::string global_full_name_for_method_borda = "Borda count with no tactical voting" ;
std::string global_full_name_for_method_irv = "Instant-Runoff Voting (IRV)" ;
std::string global_full_name_for_method_plurality = "Plurality (First Past The Post)" ;
std::string global_full_name_for_method_ple = "Pairwise Loser Elimination (PLE) pseudo-method" ;


// -----------------------------------------------
//  (Do not change these numbers.)
//  Specify constants.

const int global_question_number = 1 ;


// -----------------------------------------------
//  Define true and false constants (for easier
//  conversion between programming languages).

const int global_false = 0 ;
const int global_true = 1 ;


// -----------------------------------------------
//  Declare global variables.

int global_case_id ;
int global_case_type ;
int global_case_count_limit ;
int global_specified_choice_count ;
int global_maximum_choice_number ;
int global_choice_count_case_specific ;
int global_choice_omitted ;
int global_clone_choice_number_next ;
int global_vf_test_count ;
int global_iia_test_count ;
int global_clone_test_count ;
int global_count_of_cases_involving_tie ;


// -----------------------------------------------
//  Declare global constants.

const int global_case_all_choices = 1 ;
const int global_case_choice_omitted = 2 ;
const int global_case_choice_omitted_final = 3 ;
const int global_case_clones_included = 4 ;
const int global_case_ignored = 5 ;


// -----------------------------------------------
//  Declare frequently used variable names,
//  but these are not intended to convey data
//  between functions.

int method_id = 1 ;
int ballot_number = 0 ;
int choice_number = 0 ;
int ranking_level = 0 ;
int position_number = 0 ;
int pointer_number = 0 ;


// -----------------------------------------------
//  Declare the needed arrays.

std::string global_name_for_method[ 20 ] ;
std::string global_full_name_for_method[ 20 ] ;

int global_choice_on_ballot_at_ranking_level[ 200 ][ 20 ] ;
int calculated_vf_result_match_for_method_and_choice_count[ 20 ][ 20 ] ;
int calculated_iia_result_match_for_method_and_choice_count[ 20 ][ 20 ] ;
int calculated_clone_result_match_for_method_and_choice_count[ 20 ][ 20 ] ;
int global_choice_number_at_position[ 99 ] ;
int global_usage_count_for_choice_and_rank[ 99 ][ 99 ] ;

int global_choice_winner_all_choices_for_method[ 20 ] ;
int global_choice_winner_from_method[ 20 ] ;
int global_count_of_vf_tests_match_for_method[ 20 ] ;
int global_count_of_vf_tests_fail_match_for_method[ 20 ] ;
int global_count_of_vf_tests_tied_for_method[ 20 ] ;
int global_count_of_vf_tests_unexpected_for_method[ 20 ] ;
int global_count_of_iia_tests_match_for_method[ 20 ] ;
int global_count_of_iia_group_match_for_method[ 20 ] ;
int global_count_of_iia_tests_fail_match_for_method[ 20 ] ;
int global_count_of_iia_group_fail_match_for_method[ 20 ] ;
int global_count_of_iia_tests_tied_for_method[ 20 ] ;
int global_count_of_iia_group_tied_for_method[ 20 ] ;
int global_count_of_iia_tests_unexpected_for_method[ 20 ] ;
int global_count_of_clone_tests_match_for_method[ 20 ] ;
int global_count_of_clone_tests_fail_match_for_method[ 20 ] ;
int global_count_of_clone_tests_tied_for_method[ 20 ] ;
int global_count_of_clone_tests_clone_help_for_method[ 20 ] ;
int global_count_of_clone_tests_clone_hurt_for_method[ 20 ] ;
int global_count_of_clone_tests_unexpected_for_method[ 20 ] ;


// -----------------------------------------------
//  Specify codes that identify the meaning of the
//  next number in the coded output list.

std::string global_string_voteinfo_code_for_end_of_all_cases = "-2" ;
std::string global_string_voteinfo_code_for_case_number = "-3" ;
std::string global_string_voteinfo_code_for_question_number = "-4" ;
std::string global_string_voteinfo_code_for_number_of_choices = "-6" ;
std::string global_string_voteinfo_code_for_start_of_all_vote_info = "-7" ;
std::string global_string_voteinfo_code_for_end_of_all_vote_info = "-8" ;
std::string global_string_voteinfo_code_for_end_of_ballot = "-10" ;
std::string global_string_voteinfo_code_for_ballot_count = "-11" ;
std::string global_string_voteinfo_code_for_preference_level = "-12" ;

std::string global_voteinfo_code_for_request_instant_runoff_voting = "-50" ;
std::string global_voteinfo_code_for_request_instant_pairwise_elimination = "-51" ;
std::string global_voteinfo_code_for_request_irv_minus_pairwise_losers = "-52" ;
std::string global_voteinfo_code_for_request_star_voting = "-56" ;
std::string global_voteinfo_code_for_request_pairwise_loser_elimination = "-58" ;

const int global_voteinfo_code_for_case_number = -3 ;
const int global_voteinfo_code_for_choice = -13 ;
const int global_voteinfo_code_for_tie = -14 ;
const int global_voteinfo_code_for_start_of_votefair_popularity_ranking_sequence_results = -15 ;

const int global_voteinfo_code_for_start_of_plurality_results = -36 ;
const int global_voteinfo_code_for_end_of_plurality_results = -37 ;
const int global_voteinfo_code_for_plurality_count = -38 ;
const int global_voteinfo_code_for_winner_instant_runoff_voting = -53 ;
const int global_voteinfo_code_for_winner_instant_pairwise_elimination = -54 ;
const int global_voteinfo_code_for_winner_irv_minus_pairwise_losers = -55 ;
const int global_voteinfo_code_for_winner_star_voting = -57 ;
const int global_voteinfo_code_for_winner_pairwise_loser_elimination = -59 ;
const int global_voteinfo_code_for_winner_irv_bottom_two_runoff = -60 ;
const int global_voteinfo_code_for_winner_borda_count = -61 ;


// -----------------------------------------------
//  Declare an output file for writing log info.

std::ofstream log_out ;


// -----------------------------------------------
//  Declare an input file for reading calculated
//  results.

std::ifstream calc_results ;


// -----------------------------------------------
//  Declare the random number generator.

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator ( seed ) ;


// -----------------------------------------------
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
//  generate_preferences
//
//  This function generates random preferences
//  for the ballots.

void generate_preferences( ) {


// -----------------------------------------------
//  Initialization.

    int count_for_ranking_level ;
    int count_of_choices_not_yet_ranked ;
    int count_of_choices_need_to_rank ;

    for ( choice_number = 1 ; choice_number <= global_maximum_choice_number ; choice_number ++ )
    {
        for ( ranking_level = 1 ; ranking_level <= global_maximum_choice_number ; ranking_level ++ )
        {
            global_usage_count_for_choice_and_rank[ choice_number ][ ranking_level ] = 0 ;
        }
    }


// -----------------------------------------------
//  Begin a loop that handles one ballot.

    for ( ballot_number = 1 ; ballot_number <= global_maximum_ballot_number ; ballot_number ++ )
    {


// -----------------------------------------------
//  Put the choice numbers in a list so that
//  they can be chosen at random without repeating
//  any choice number.

        for ( choice_number = 1 ; choice_number <= global_maximum_choice_number ; choice_number ++ )
        {
            global_choice_number_at_position[ choice_number ] = choice_number ;
        }


// -----------------------------------------------
//  Specify the number of choices that need to be
//  randomly assigned to a ranking level.
//  If clone independence is being measured, the
//  ranking of the clones is just after choice
//  number one, so the clone choices are not
//  assigned randomly.

        count_of_choices_need_to_rank = global_maximum_choice_number ;
//        log_out << "{" ;


// -----------------------------------------------
//  Begin a loop that handles each ranking level.

        ranking_level = 1 ;
        count_of_choices_not_yet_ranked = count_of_choices_need_to_rank ;
        for ( count_for_ranking_level = 1 ; count_for_ranking_level <= count_of_choices_need_to_rank ; count_for_ranking_level ++ )
        {


// -----------------------------------------------
//  Randomly choose a choice number for this
//  ranking level.  Do not repeat any choice number
//  already used at a previous ranking level.

            std::uniform_int_distribution<int> distribution( 1 , count_of_choices_not_yet_ranked ) ;
            position_number = distribution( generator ) ;
            choice_number = global_choice_number_at_position[ position_number ] ;
            global_choice_on_ballot_at_ranking_level[ ballot_number ][ ranking_level ] = choice_number ;
//            log_out << choice_number ;


// -----------------------------------------------
//  Keep track of choice number usage to allow
//  verifying randomness.

            global_usage_count_for_choice_and_rank[ choice_number ][ ranking_level ] ++ ;


// -----------------------------------------------
//  Update the list of which choices still need to
//  be assigned to a ranking level.

//            log_out << " " << position_number << ":" ;
            if ( position_number < count_of_choices_not_yet_ranked )
            {
                for ( pointer_number = position_number ; pointer_number <= ( global_maximum_choice_number - 1 ) ; pointer_number ++ )
                {
                    global_choice_number_at_position[ pointer_number ] = global_choice_number_at_position[ pointer_number + 1 ] ;
//                    log_out << global_choice_number_at_position[ pointer_number ] ;
                }
            }
//            log_out << ":" << " " ;


// -----------------------------------------------
//  Repeat the loop that handles one ranking level.

            ranking_level ++ ;
            count_of_choices_not_yet_ranked -- ;
        }

//        log_out << "}" ;


// -----------------------------------------------
//  Repeat the loop that handles one ballot.

    }


// -----------------------------------------------
//  If desired, write to a file the counts that
//  can be looked at to verify that the preference
//  info is really random.
//  Use either "1 == 1" or "1 == 0" to generate or
//  omit this output file.

    if ( 1 == 1 )
    {
        std::fstream verifyfile;
        verifyfile.open ( "temp_verify_randomness.txt" , std::fstream::app ) ;
        for ( choice_number = 1 ; choice_number <= global_maximum_choice_number ; choice_number ++ )
        {
            for ( ranking_level = 1 ; ranking_level <= global_maximum_choice_number ; ranking_level ++ )
            {
                verifyfile << "choice " << choice_number << " rank " << ranking_level << " usage " << global_usage_count_for_choice_and_rank[ choice_number ][ ranking_level ] << std::endl ;
            }
        }
        verifyfile.close( ) ;
    }


// -----------------------------------------------
//  End of function generate_preferences.

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//     handle_calculated_results
//
//  Reads numbers and codes from the file
//  written by the calculation program.
//
// -----------------------------------------------
// -----------------------------------------------

void handle_calculated_results( )
{

    std::string input_line ;
    std::string input_text_word ;


// -----------------------------------------------
//  Initialization.

    int method_id = 1 ;
    int current_result_code = 0 ;
    int previous_result_code = 0 ;
    int next_result_code = 0 ;
    int count_of_result_codes = 0 ;
    int count_position_at_start_of_votefair_popularity_sequence = 0 ;
    int count_position_at_start_of_plurality_result = 0 ;
    int count_position_at_choice_number = 0 ;
    int choice_number_adjustment = 0 ;
    int true_or_false_within_plurality_result = global_false ;
    int plurality_choice_number = 0 ;
    int plurality_winner = global_voteinfo_code_for_tie ;
    int highest_plurality_count = -1 ;
    for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
    {
        global_choice_winner_from_method[ method_id ] = 0 ;
    }


// -----------------------------------------------
//  Begin loop to handle one line from the input file
//  that contains the calculated results.

    log_out << std::endl << "[" << global_case_id << "][t " << global_case_type << "][ch " << global_choice_count_case_specific << "][om " << global_choice_omitted << "]" ;
    count_position_at_start_of_votefair_popularity_sequence = -10 ;
    count_position_at_choice_number = -10 ;
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
//  Begin loop to get first/next space-delimited
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
//  output files and then exit the program.

            try
            {
                current_result_code = convert_text_to_integer( pointer_to_word ) ;
            }
            catch( ... )
            {
                log_out << "Error: invalid input word: " << pointer_to_word << std::endl ;
                log_out << "[Warning: Input line contains non-numeric characters (" << pointer_to_word << "), so this case (" << global_case_id << ") cannot be calculated]" << std::endl ;
                exit( EXIT_FAILURE ) ;
            }


// -----------------------------------------------
//  Handle the supplied vote-info number.

//            log_out << "[" << current_result_code << "]" ;

            if ( current_result_code == global_voteinfo_code_for_case_number )
            {
                for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
                {
                    global_choice_winner_from_method[ method_id ] = 0 ;
                }

            } else if ( current_result_code == global_voteinfo_code_for_start_of_votefair_popularity_ranking_sequence_results )
            {
                count_position_at_start_of_votefair_popularity_sequence = count_of_result_codes ;

            } else if ( current_result_code == global_voteinfo_code_for_choice )
            {
                count_position_at_choice_number = count_of_result_codes ;

            } else if ( ( count_of_result_codes == count_position_at_start_of_votefair_popularity_sequence + 2 ) && ( count_of_result_codes == count_position_at_choice_number + 1 ) )
            {
                global_choice_winner_from_method[ global_method_votefair ] = current_result_code ;
                log_out << "[" << global_name_for_method[ global_method_votefair ] << " " << global_choice_winner_from_method[ global_method_votefair ] << "]" ;

            } else if ( ( current_result_code == global_voteinfo_code_for_tie ) && ( count_of_result_codes == count_position_at_start_of_votefair_popularity_sequence + 3 ) )
            {
                global_choice_winner_from_method[ global_method_votefair ] = 0 ;
                log_out << "[" << global_name_for_method[ global_method_votefair ] << "_tie]" ;

            } else if ( previous_result_code == global_voteinfo_code_for_winner_instant_pairwise_elimination )
            {
                global_choice_winner_from_method[ global_method_ipe ] = current_result_code ;
                log_out << "[" << global_name_for_method[ global_method_ipe ] << " " << global_choice_winner_from_method[ global_method_ipe ] << "]" ;

            } else if ( previous_result_code == global_voteinfo_code_for_winner_irv_minus_pairwise_losers )
            {
                global_choice_winner_from_method[ global_method_rcipe ] = current_result_code ;
                log_out << "[" << global_name_for_method[ global_method_rcipe ] << " " << global_choice_winner_from_method[ global_method_rcipe ] << "]" ;

            } else if ( previous_result_code == global_voteinfo_code_for_winner_instant_runoff_voting )
            {
                global_choice_winner_from_method[ global_method_irv ] = current_result_code ;
                log_out << "[" << global_name_for_method[ global_method_irv ] << " " << global_choice_winner_from_method[ global_method_irv ] << "]" ;

            } else if ( previous_result_code == global_voteinfo_code_for_winner_irv_bottom_two_runoff )
            {
                global_choice_winner_from_method[ global_method_irvbtr ] = current_result_code ;
                log_out << "[" << global_name_for_method[ global_method_irvbtr ] << " " << global_choice_winner_from_method[ global_method_irvbtr ] << "]" ;

            } else if ( previous_result_code == global_voteinfo_code_for_winner_borda_count )
            {
                global_choice_winner_from_method[ global_method_borda ] = current_result_code ;
                log_out << "[" << global_name_for_method[ global_method_borda ] << " " << global_choice_winner_from_method[ global_method_borda ] << "]" ;

            } else if ( previous_result_code == global_voteinfo_code_for_winner_star_voting )
            {
                global_choice_winner_from_method[ global_method_star ] = current_result_code ;
                log_out << "[" << global_name_for_method[ global_method_star ] << " " << global_choice_winner_from_method[ global_method_star ] << "]" ;

            } else if ( previous_result_code == global_voteinfo_code_for_winner_pairwise_loser_elimination )
            {
                global_choice_winner_from_method[ global_method_ple ] = current_result_code ;
                log_out << "[" << global_name_for_method[ global_method_ple ] << " " << global_choice_winner_from_method[ global_method_ple ] << "]" ;

            } else if ( current_result_code == global_voteinfo_code_for_start_of_plurality_results )
            {
                true_or_false_within_plurality_result = global_true ;

            } else if ( ( true_or_false_within_plurality_result == global_true ) && ( count_of_result_codes == count_position_at_choice_number + 1 ) )
            {
               plurality_choice_number = current_result_code ;

            } else if ( ( true_or_false_within_plurality_result == global_true ) && ( previous_result_code == global_voteinfo_code_for_plurality_count ) )
            {
                if ( current_result_code > highest_plurality_count )
                {
                    plurality_winner = plurality_choice_number ;
                    highest_plurality_count = current_result_code ;
                } else if ( current_result_code == highest_plurality_count )
                {
                    plurality_winner = global_voteinfo_code_for_tie ;
                }

            } else if ( current_result_code == global_voteinfo_code_for_end_of_plurality_results )
            {
                global_choice_winner_from_method[ global_method_plurality ] = plurality_winner ;
                log_out << "[" << global_name_for_method[ global_method_plurality ] << " " << global_choice_winner_from_method[ global_method_plurality ] << "]" ;
                true_or_false_within_plurality_result = global_false ;

            }


// -----------------------------------------------
//  Repeat the loop for the next word (within the line).

            count_of_result_codes ++ ;
            previous_result_code = current_result_code ;
            pointer_to_word = strtok( NULL, " ,." ) ;
        }


// -----------------------------------------------
//  Repeat the loop for the next line of data from
//  the input file.

    }


// -----------------------------------------------
//  If this case uses all the choices and a tie
//  occurs in VoteFair popularity ranking, or in
//  plurality counting, or in IRV, the case is
//  ignored because this would not be a meaningful
//  test.

    if ( ( global_case_type == global_case_all_choices ) && ( ( global_choice_winner_from_method[ global_method_votefair ] < 1 ) || ( global_choice_winner_from_method[ global_method_plurality ] < 1 ) || ( global_choice_winner_from_method[ global_method_irv ] < 1 ) ) )
    {
        global_count_of_cases_involving_tie ++ ;
        global_case_type = global_case_ignored ;
        log_out << "[ignored]" ;
        return ;
    }


// -----------------------------------------------
//  If this case included all the choices, save
//  the winner based on each method.

    if ( global_case_type == global_case_all_choices )
    {
        for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
        {
            global_choice_winner_all_choices_for_method[ method_id ] = global_choice_winner_from_method[ method_id ] ;
        }
    }


// -----------------------------------------------
//  Count the number of cases that match, or fail
//  to match, the winner according to VoteFair
//  popularity ranking.  Also count ties.

    if ( global_case_type == global_case_all_choices )
    {
        global_vf_test_count ++ ;
        for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
        {
            if ( global_choice_winner_from_method[ method_id ] == global_choice_winner_from_method[ global_method_votefair ] )
            {
                global_count_of_vf_tests_match_for_method[ method_id ] ++ ;
            } else if ( global_choice_winner_from_method[ method_id ] > 0 )
            {
                global_count_of_vf_tests_fail_match_for_method[ method_id ] ++ ;
                log_out << "[" << global_name_for_method[ method_id ] << " fails]" ;
            } else
            {
                global_count_of_vf_tests_tied_for_method[ method_id ] ++ ;
                log_out << "[" << global_name_for_method[ method_id ] << " tied]" ;
            }
        }
    }


// -----------------------------------------------
//  When testing for independence of irrelevant
//  alternatives, if the winner of the case with
//  all the choices is not the omitted choice and
//  is not the same as the winner without the
//  omitted choice, then count this case as a
//  failure for the counting method.

    if ( ( global_case_type == global_case_choice_omitted ) || ( global_case_type == global_case_choice_omitted_final ) )
    {
        for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
        {
            if ( global_choice_winner_all_choices_for_method[ method_id ] == global_choice_omitted )
            {
                continue ;
            }
            choice_number_adjustment = 0 ;
            if ( ( global_choice_winner_from_method[ method_id ] >= global_choice_omitted ) )
            {
                choice_number_adjustment = 1 ;
            }
            if ( global_choice_winner_from_method[ method_id ] + choice_number_adjustment == global_choice_winner_all_choices_for_method[ method_id ] )
            {
                global_count_of_iia_group_match_for_method[ method_id ] ++ ;
            } else if ( global_choice_winner_from_method[ method_id ] < 1 )
            {
                global_count_of_iia_group_tied_for_method[ method_id ] ++ ;
                log_out << "[" << global_name_for_method[ method_id ] << " IIA tied]" ;
            } else
            {
                global_count_of_iia_group_fail_match_for_method[ method_id ] ++ ;
                log_out << "[" << global_name_for_method[ method_id ] << " IIA fails]" ;
            }
        }
    }


// -----------------------------------------------
//  For testing independence of irrelevant
//  alternatives, determine the overall result of
//  the test according to what happened when
//  testing the removal of each choice (one at a
//  time).  The test fails if the winner changes
//  as a result of omitting any choice other than
//  the original winning choice.

    if ( global_case_type == global_case_choice_omitted_final )
    {
        global_iia_test_count ++ ;
        for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
        {
            if ( global_count_of_iia_group_fail_match_for_method[ method_id ] > 0 )
            {
                global_count_of_iia_tests_fail_match_for_method[ method_id ] ++ ;
            } else if ( global_count_of_iia_group_match_for_method[ method_id ] > 0 )
            {
                global_count_of_iia_tests_match_for_method[ method_id ] ++ ;
            } else
            {
                global_count_of_iia_tests_tied_for_method[ method_id ] ++ ;
            }
        }
    }


// -----------------------------------------------
//  If testing clone independence, determine
//  whether the winner is the same as the case
//  with all the choices.  If not, track
//  whether the added clone helps or hurts the
//  similar choice.

    if ( global_case_type == global_case_clones_included )
    {
        global_clone_test_count ++ ;
        for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
        {
            if ( ( global_choice_winner_from_method[ method_id ] > 0 ) && ( global_choice_winner_from_method[ method_id ] == global_choice_winner_all_choices_for_method[ method_id ] ) )
            {
                global_count_of_clone_tests_match_for_method[ method_id ] ++ ;
            } else if ( global_choice_winner_from_method[ method_id ] < 1 )
            {
                global_count_of_clone_tests_tied_for_method[ method_id ] ++ ;
                log_out << "[" << global_name_for_method[ method_id ] << " CL tied]" ;
            } else
            {
                global_count_of_clone_tests_fail_match_for_method[ method_id ] ++ ;
                log_out << "[" << global_name_for_method[ method_id ] << " CL fails]" ;
                if ( global_choice_winner_all_choices_for_method[ method_id ] == 1 )
                {
                    global_count_of_clone_tests_clone_hurt_for_method[ method_id ] ++ ;
                    log_out << "[" << global_name_for_method[ method_id ] << " hurts]" ;
                }
                if ( global_choice_winner_from_method[ method_id ] == 1 )
                {
                    global_count_of_clone_tests_clone_help_for_method[ method_id ] ++ ;
                    log_out << "[" << global_name_for_method[ method_id ] << " helps]" ;
                }
            }
        }
    }


// -----------------------------------------------
//  Identify interesting cases:

    if ( ( global_choice_winner_from_method[ global_method_ple ] < 0 ) && ( global_choice_winner_from_method[ global_method_rcipe ] > 0 ) && ( global_choice_winner_from_method[ global_method_ipe ] > 0 ) && ( global_choice_winner_from_method[ global_method_irv ] > 0 ) && ( global_choice_winner_from_method[ global_method_votefair ] == global_choice_winner_from_method[ global_method_rcipe ] ) && ( global_choice_winner_from_method[ global_method_rcipe ] != global_choice_winner_from_method[ global_method_irv ] ) )
    {
        log_out << "[interesting]" ;
        if ( global_choice_winner_from_method[ global_method_plurality ] != global_choice_winner_from_method[ global_method_votefair ] )
        {
            log_out << "[very]" ;
        }
    }


// -----------------------------------------------
//  End of function handle_calculated_results

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//     write_test_results
//
//  Writes the test results.
//
// -----------------------------------------------
// -----------------------------------------------

void write_test_results( )
{


// -----------------------------------------------
//  Initialization.

    int method_id = 0 ;
    int count_of_vf_tests = 0 ;
    int calculated_vf_result_match = 0 ;
    int calculated_vf_result_fail_match = 0 ;
    int calculated_vf_result_ties = 0 ;
    int calculated_vf_result_match_no_ties = 0 ;
    int count_of_iia_tests = 0 ;
    int calculated_iia_result_match = 0 ;
    int calculated_iia_result_fail_match = 0 ;
    int calculated_iia_result_ties = 0 ;
    int calculated_iia_result_match_no_ties = 0 ;
    int count_of_clone_tests = 0 ;
    int calculated_clone_result_match = 0 ;
    int calculated_clone_result_clone_help = 0 ;
    int calculated_clone_result_clone_hurt = 0 ;
    int calculated_clone_result_fail_match = 0 ;
    int calculated_clone_result_ties = 0 ;
    int calculated_clone_result_match_no_ties = 0 ;


// -----------------------------------------------
//  Begin to write the results.

    log_out << std::endl << std::endl << std::endl ;

    log_out << "TEST CONDITIONS:" << std::endl ;
    log_out << "number of choices: " << global_maximum_choice_number << std::endl ;
    log_out << "number of ballots: " << global_maximum_ballot_number << std::endl ;
    log_out << "number of clones (excluding original): " << global_number_of_clones << std::endl ;
    log_out << "number of VF-match tests: " << global_vf_test_count << std::endl ;
    log_out << "number of IIA tests: " << global_iia_test_count << std::endl ;
    log_out << "number of clone independence tests: " << global_clone_test_count << std::endl ;
    log_out << "number of cases limit per choice count: " << global_maximum_case_count_per_choice_count << std::endl ;
    log_out << "number of cases dismissed because of tied result: " << global_count_of_cases_involving_tie << std::endl ;
    log_out << "PLUR method ignores all but first-ranked choice" << std::endl << std::endl << std::endl ;


// -----------------------------------------------
//  For tests of independence of irrelevant
//  alternatives, calculate and write those results.

    log_out << "Independence of Irrelevant Alternatives (IIA) success and failure rates:" << std::endl ;
    if ( global_iia_test_count > 0 )
    {
        for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
        {
            calculated_iia_result_match = int( ( 100 * global_count_of_iia_tests_match_for_method[ method_id ] ) / global_iia_test_count ) ;
            calculated_iia_result_match_for_method_and_choice_count[ method_id ][ global_maximum_choice_number ] = calculated_iia_result_match ;
            calculated_iia_result_fail_match = int( ( 100 *  global_count_of_iia_tests_fail_match_for_method[ method_id ] ) / global_iia_test_count ) ;
            calculated_iia_result_ties = int( ( 100 *  global_count_of_iia_tests_tied_for_method[ method_id ] ) / global_iia_test_count ) ;
            log_out << global_name_for_method[ method_id ] << " agree/disagree/tie: " << calculated_iia_result_match << "  " << calculated_iia_result_fail_match << "  " << calculated_iia_result_ties << std::endl ;
            if ( global_count_of_iia_tests_unexpected_for_method[ method_id ] > 0 )
            {
                log_out << "unexpected IIA test results count: " << global_count_of_iia_tests_unexpected_for_method[ method_id ] << std::endl << std::endl ;
            }
        }
    } else
    {
        log_out << global_name_for_method[ method_id ] << " has zero IIA test count" << std::endl ;
    }
    log_out << std::endl << std::endl ;


// -----------------------------------------------
//  For tests of clone independence, calculate
//  and write those results.

    log_out << "Clone Independence success and failure rates:" << std::endl ;
    if ( global_clone_test_count > 0 )
    {
        for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
        {
            calculated_clone_result_match = int( ( 100 * global_count_of_clone_tests_match_for_method[ method_id ] ) / global_clone_test_count ) ;
            calculated_clone_result_match_for_method_and_choice_count[ method_id ][ global_maximum_choice_number ] = calculated_clone_result_match ;
            calculated_clone_result_fail_match = int( ( 100 *  global_count_of_clone_tests_fail_match_for_method[ method_id ] ) / global_clone_test_count ) ;
            calculated_clone_result_ties = int( ( 100 *  global_count_of_clone_tests_tied_for_method[ method_id ] ) / global_clone_test_count ) ;
            calculated_clone_result_clone_help = int( ( 100 *  global_count_of_clone_tests_clone_help_for_method[ method_id ] ) / global_clone_test_count ) ;
            calculated_clone_result_clone_hurt = int( ( 100 *  global_count_of_clone_tests_clone_hurt_for_method[ method_id ] ) / global_clone_test_count ) ;
            log_out << global_name_for_method[ method_id ] << " agree/disagree/tie (help) (hurt): " << calculated_clone_result_match << "  " << calculated_clone_result_fail_match << "  " << calculated_clone_result_ties << "  (" << calculated_clone_result_clone_help << ")  (" << calculated_clone_result_clone_hurt << ")" << std::endl ;
            if ( global_count_of_clone_tests_unexpected_for_method[ method_id ] > 0 )
            {
                log_out << "unexpected clone test results count: " << global_count_of_clone_tests_unexpected_for_method[ method_id ] << std::endl ;
            }
        }
    } else
    {
        log_out << global_name_for_method[ method_id ] << " has zero clone test count" ;
    }
    log_out << std::endl << std::endl ;


// -----------------------------------------------
//  For test comparisons with VoteFair ranking,
//  calculate and write those results.

    log_out << "Match with VoteFair popularity ranking result?" << std::endl ;
    if ( global_vf_test_count > 0 )
    {
        for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
        {
            calculated_vf_result_match = int( ( 100 * global_count_of_vf_tests_match_for_method[ method_id ] ) / global_vf_test_count ) ;
            calculated_vf_result_match_for_method_and_choice_count[ method_id ][ global_maximum_choice_number ] = calculated_vf_result_match ;
            calculated_vf_result_fail_match = int( ( 100 * global_count_of_vf_tests_fail_match_for_method[ method_id ] ) / global_vf_test_count ) ;
            calculated_vf_result_ties = int( ( 100 * global_count_of_vf_tests_tied_for_method[ method_id ] ) / global_vf_test_count ) ;
            log_out << global_name_for_method[ method_id ] << " agree/disagree/tied: " << calculated_vf_result_match << "  " << calculated_vf_result_fail_match << "  " << calculated_vf_result_ties << std::endl ;
            if ( global_count_of_vf_tests_unexpected_for_method[ method_id ] > 0 )
            {
                log_out << "unexpected VF test results count: " << global_count_of_vf_tests_unexpected_for_method[ method_id ] << std::endl ;
            }
        }
    } else
    {
        log_out << global_name_for_method[ method_id ] << " has zero VF test count" << std::endl ;
    }

    log_out << std::endl << std::endl ;


// -----------------------------------------------
//  End of function write_test_results

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//     do_all_tests_for_specified_choice_count
//
//  Do all the tests using a specified number of
//  choices (candidates if this is simulating
//  a governmental election).
//
// -----------------------------------------------
// -----------------------------------------------


void do_all_tests_for_specified_choice_count( ) {


// -----------------------------------------------
//  Specify the number of choices for this series
//  of tests.

    global_maximum_choice_number = global_specified_choice_count ;


// -----------------------------------------------
//  Initialization.

    int integer_from_system_call = 0 ;
    int counter = 0 ;
    int case_count = 0 ;
    for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
    {
        global_choice_winner_all_choices_for_method[ method_id ] = 0 ;
        global_choice_winner_from_method[ method_id ] = 0 ;
        global_count_of_vf_tests_match_for_method[ method_id ] = 0 ;
        global_count_of_vf_tests_fail_match_for_method[ method_id ] = 0 ;
        global_count_of_vf_tests_tied_for_method[ method_id ] = 0 ;
        global_count_of_vf_tests_unexpected_for_method[ method_id ] = 0 ;
        global_count_of_iia_tests_match_for_method[ method_id ] = 0 ;
        global_count_of_iia_group_match_for_method[ method_id ] = 0 ;
        global_count_of_iia_tests_fail_match_for_method[ method_id ] = 0 ;
        global_count_of_iia_group_fail_match_for_method[ method_id ] = 0 ;
        global_count_of_iia_tests_tied_for_method[ method_id ] = 0 ;
        global_count_of_iia_group_tied_for_method[ method_id ] = 0 ;
        global_count_of_iia_tests_unexpected_for_method[ method_id ] = 0 ;
        global_count_of_clone_tests_match_for_method[ method_id ] = 0 ;
        global_count_of_clone_tests_fail_match_for_method[ method_id ] = 0 ;
        global_count_of_clone_tests_tied_for_method[ method_id ] = 0 ;
        global_count_of_clone_tests_clone_help_for_method[ method_id ] = 0 ;
        global_count_of_clone_tests_clone_hurt_for_method[ method_id ] = 0 ;
        global_count_of_clone_tests_unexpected_for_method[ method_id ] = 0 ;
    }
    global_vf_test_count = 0 ;
    global_iia_test_count = 0 ;
    global_clone_test_count = 0 ;
    global_count_of_cases_involving_tie = 0 ;
    global_case_type = global_case_all_choices ;
    global_choice_omitted = 0 ;
    global_clone_choice_number_next = 0 ;


// -----------------------------------------------
//  Begin a loop that handles one case.
//  Some tests require multiple cases for each test.
//  Each case sends ballot information to the
//  software that reads the ballot information and
//  identifies which choice deserves to win
//  according to different vote-counting methods.

    for ( case_count = 1 ; case_count <= global_case_count_limit ; case_count ++ )
    {
        std::cout << "." ;

//        log_out << "Case type: " << global_case_type << std::endl ;


// -----------------------------------------------
//  When starting a new series of related tests,
//  initialize some values.

        if ( global_case_type == global_case_all_choices )
        {
            global_choice_omitted = 0 ;
            global_clone_choice_number_next = 0 ;
            for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
            {
                global_count_of_iia_group_match_for_method[ method_id ] = 0 ;
                global_count_of_iia_group_fail_match_for_method[ method_id ] = 0 ;
                global_count_of_iia_group_tied_for_method[ method_id ] = 0 ;
            }
        }


// -----------------------------------------------
//  Specify the number of choices for this case.

        global_choice_count_case_specific = global_maximum_choice_number ;
        if ( ( global_case_type == global_case_choice_omitted ) || ( global_case_type == global_case_choice_omitted_final ) )
        {
            global_choice_count_case_specific = global_maximum_choice_number - 1 ;
        }
        if ( global_case_type == global_case_clones_included )
        {
            global_choice_count_case_specific = global_maximum_choice_number + global_number_of_clones ;
        }


// -----------------------------------------------
//  When needed, randomly generate new ballot
//  rankings.

        if ( global_case_type == global_case_all_choices )
        {
            generate_preferences( ) ;
        }


// -----------------------------------------------
//  Write the beginning of a new output file.

        std::fstream outfile ;
        outfile.open ( "temp_generated_random_ballots.txt" , std::fstream::out ) ;
        outfile << global_string_voteinfo_code_for_start_of_all_vote_info << std::endl ;
        outfile << global_string_voteinfo_code_for_case_number << " " << global_case_id << std::endl ;
        outfile << global_voteinfo_code_for_request_instant_pairwise_elimination << std::endl ;
        outfile << global_voteinfo_code_for_request_instant_runoff_voting << std::endl ;
        outfile << global_voteinfo_code_for_request_irv_minus_pairwise_losers << std::endl ;
        outfile << global_voteinfo_code_for_request_star_voting << std::endl ;
        outfile << global_voteinfo_code_for_request_pairwise_loser_elimination << std::endl ;
        outfile << global_string_voteinfo_code_for_question_number << " " << global_question_number << std::endl ;
        outfile << global_string_voteinfo_code_for_number_of_choices << " " << global_choice_count_case_specific << std::endl ;


// -----------------------------------------------
//  Begin a loop that handles one ballot.
//  Write the beginning of the info for this ballot.

//        log_out << std::endl ;
        for ( ballot_number = 1 ; ballot_number <= global_maximum_ballot_number ; ballot_number ++ )
        {
            outfile << global_string_voteinfo_code_for_ballot_count << " 1" << std::endl ;
            outfile << global_string_voteinfo_code_for_question_number << " " << global_question_number << std::endl ;


// -----------------------------------------------
//  Begin a loop that handles one ranking level.

//            log_out << "{" ;
            for ( ranking_level = 1 ; ranking_level <= global_maximum_choice_number ; ranking_level ++ )
            {
//                log_out << "||" ;


// -----------------------------------------------
//  Get the choice number at the current ranking
//  level.

                choice_number = global_choice_on_ballot_at_ranking_level[ ballot_number ][ ranking_level ] ;


// -----------------------------------------------
//  If this case includes all the normal choices,
//  write the choice number that is at the
//  next-lower preference level.

                if ( global_case_type == global_case_all_choices )
                {
                    outfile << choice_number << std::endl ;
//                    log_out << choice_number ;
                }


// -----------------------------------------------
//  If this case tests for independence of
//  irrelevant alternatives, and the choice at
//  this ranking level is not omitted, write the
//  equivalent -- possibly adjusted -- choice
//  number.

                if ( ( global_case_type == global_case_choice_omitted ) || ( global_case_type == global_case_choice_omitted_final ) )
                {
                    if ( choice_number < global_choice_omitted )
                    {
                        outfile << choice_number << std::endl ;
//                        log_out << choice_number ;
                    } else if ( choice_number > global_choice_omitted )
                    {
                        outfile << ( choice_number - 1 ) << std::endl ;
//                        log_out << choice_number ;
                    }
                }


// -----------------------------------------------
//  If clone independence is being tested, and if
//  this case includes the clone choices, and if
//  the current choice is the original similar
//  choice -- specifically choice number one --
//  then write the choice numbers of the clones
//  and the original choice, but rotate the
//  ranking levels of the clone & similar choices.
//  This rotation simulates voters randomly
//  chosing one of the clones as each voter's
//  favorite similar choice.

//  todo: ensure that first ballot ranks choice 1 higher than its clones

                if ( global_case_type == global_case_clones_included )
                {
                    if ( choice_number == 1 )
                    {
                        for ( counter = 0 ; counter <= global_number_of_clones ; counter ++ )
                        {
                            choice_number = global_maximum_choice_number + counter + global_clone_choice_number_next ;
                            if ( choice_number > global_maximum_choice_number + global_number_of_clones )
                            {
                                choice_number = choice_number - global_number_of_clones - 1 ;
                            }
                            if ( choice_number == global_maximum_choice_number )
                            {
                                choice_number = 1 ;
                            }
                            outfile << choice_number << std::endl ;
//                            log_out << choice_number << " " ;
                        }
                        global_clone_choice_number_next ++ ;
                        if ( global_clone_choice_number_next > global_number_of_clones )
                        {
                            global_clone_choice_number_next = 0 ;
                        }
                    } else
                    {
                        outfile << choice_number << std::endl ;
//                        log_out << choice_number << " " ;
                    }
                }


// -----------------------------------------------
//  Repeat the loop that handles one preference ranking.

            }
//            log_out << "}" ;


// -----------------------------------------------
//  Write the code that terminates this ballot.

            outfile << global_string_voteinfo_code_for_end_of_ballot << std::endl ;


// -----------------------------------------------
//  Repeat the loop that handles one ballot.

        }


// -----------------------------------------------
//  Write the codes at the end of the output file.

        outfile << global_string_voteinfo_code_for_end_of_all_vote_info << std::endl ;
        outfile << global_string_voteinfo_code_for_end_of_all_cases << std::endl ;


// -----------------------------------------------
//  Close the output file that contains the ballots.

        outfile.close( ) ;


// -----------------------------------------------
//  Run the VoteFair-ranking-cpp program.
//  It uses as input the file that was just written.
//
//  documentation:  https://cplusplus.com/reference/cstdlib/system/

        integer_from_system_call = system( ".\\votefair_ranking < temp_generated_random_ballots.txt > temp_votefair_ranking_output.txt" ) ;
        outfile << integer_from_system_call << std::endl ;


// -----------------------------------------------
//  Join the log files to allow viewing details
//  of specific cases of interest.

        integer_from_system_call = system( "type output_votefair_ranking_log.txt >> temp_joined_output_votefair_ranking_log.txt" ) ;


// -----------------------------------------------
//  Open the calculated-results file for reading,
//  then read the calculated results, and count the
//  number of matches, match failures, and tied
//  calculated results.

        calc_results.open ( "temp_votefair_ranking_output.txt" , std::ios::in ) ;
        handle_calculated_results( ) ;
        calc_results.close ( ) ;


// -----------------------------------------------
//  Update the case number.

        global_case_id ++ ;


// -----------------------------------------------
//  Determine which kind of case is needed next.
//  If an independence of irrelevant alternatives
//  test is being done, update the choice omitted
//  number.

        if ( global_case_type == global_case_all_choices )
        {
            global_case_type = global_case_choice_omitted ;
            global_choice_omitted = 1 ;
        } else if ( ( global_case_type == global_case_choice_omitted ) && ( global_choice_omitted == ( global_maximum_choice_number - 1 ) ) )
        {
            global_case_type = global_case_choice_omitted_final ;
            global_choice_omitted = global_maximum_choice_number ;
        } else if ( global_case_type == global_case_choice_omitted )
        {
            global_choice_omitted ++ ;
        } else if ( global_case_type == global_case_choice_omitted_final )
        {
            global_case_type = global_case_clones_included ;
            global_choice_omitted = 0 ;
        } else
        {
            global_case_type = global_case_all_choices ;
            global_choice_omitted = 0 ;
        }


// -----------------------------------------------
//  Repeat the loop that handles one case.

    }


// -----------------------------------------------
//  Write the results.

    write_test_results( ) ;


// -----------------------------------------------
//  End of function do_all_tests_for_specified_choice_count

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//     write_final_results
//
//  Write the final results in a way that
//  spreadsheet software can convert into charts.
//
// -----------------------------------------------
// -----------------------------------------------

void write_final_results( )
{

    int test_type ;
    int method_id ;
    int choice_count ;


// -----------------------------------------------
//  Write the final results in "comma-separated
//  variables" (CSV) format.

    log_out << "Summary in spreadsheet-chartable format:" << std::endl << std::endl ;

    for ( test_type = 1 ; test_type <= 3 ; test_type ++ )
    {
        if ( test_type == 1 )
        {
            log_out << "Success rates for Independence of Irrelevant Alternatives (IIA)" << std::endl ;
        } else if ( test_type == 2 )
        {
            log_out << "Success rates for Clone Independence" << std::endl ;
        } else
        {
            log_out << "Success rates for matching the Condorcet-Kemeny method" << std::endl ;
        }
        for ( choice_count = global_choice_count_minimum ; choice_count <= global_choice_count_maximum ; choice_count ++ )
        {
            log_out << "," << choice_count ;
        }
        log_out << std::endl ;
        for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
        {
            log_out << global_full_name_for_method[ method_id ] ;
            for ( choice_count = global_choice_count_minimum ; choice_count <= global_choice_count_maximum ; choice_count ++ )
            {
                if ( test_type == 1 )
                {
                    log_out << "," << calculated_iia_result_match_for_method_and_choice_count[ method_id ][ choice_count ] ;
                } else if ( test_type == 2 )
                {
                    log_out << "," << calculated_clone_result_match_for_method_and_choice_count[ method_id ][ choice_count ] ;
                } else
                {
                    log_out << "," << calculated_vf_result_match_for_method_and_choice_count[ method_id ][ choice_count ] ;
                }
            }
            log_out << std::endl ;
        }
        log_out << std::endl << std::endl ;
    }


// -----------------------------------------------
//  End of function write_final_results

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//  Execution starts here.

int main( ) {


// -----------------------------------------------
//  Initialization.

    global_case_id = global_minimum_case_id ;
    global_name_for_method[ global_method_votefair ] = global_name_for_method_votefair ;
    global_name_for_method[ global_method_ipe ] = global_name_for_method_ipe ;
    global_name_for_method[ global_method_rcipe ] = global_name_for_method_rcipe ;
    global_name_for_method[ global_method_star ] = global_name_for_method_star ;
    global_name_for_method[ global_method_irv ] = global_name_for_method_irv ;
    global_name_for_method[ global_method_irvbtr ] = global_name_for_method_irvbtr ;
    global_name_for_method[ global_method_borda ] = global_name_for_method_borda ;
    global_name_for_method[ global_method_ple ] = global_name_for_method_ple ;
    global_name_for_method[ global_method_plurality ] = global_name_for_method_plurality ;
    global_full_name_for_method[ global_method_votefair ] = global_full_name_for_method_votefair ;
    global_full_name_for_method[ global_method_ipe ] = global_full_name_for_method_ipe ;
    global_full_name_for_method[ global_method_rcipe ] = global_full_name_for_method_rcipe ;
    global_full_name_for_method[ global_method_star ] = global_full_name_for_method_star ;
    global_full_name_for_method[ global_method_irv ] = global_full_name_for_method_irv ;
    global_full_name_for_method[ global_method_irvbtr ] = global_full_name_for_method_irvbtr ;
    global_full_name_for_method[ global_method_borda ] = global_full_name_for_method_borda ;
    global_full_name_for_method[ global_method_ple ] = global_full_name_for_method_ple ;
    global_full_name_for_method[ global_method_plurality ] = global_full_name_for_method_plurality ;


// -----------------------------------------------
//  Open the output log file.

    log_out.open ( "temp_log_from_generate_random_ballots.txt" , std::ios::out ) ;


// -----------------------------------------------
//  Verify that the specified choice count
//  maximum and minimum values are reasonable.

    if ( global_choice_count_minimum > global_choice_count_maximum )
    {
        log_out << "ERROR: Choice count minimum (" << global_choice_count_minimum << ") exceeds choice count maximum (" << global_choice_count_maximum << ")" << std::endl ;
        return 0 ;
    } else if ( global_choice_count_minimum < 2 )
    {
        log_out << "ERROR: Choice count minimum (" << global_choice_count_minimum << ") is less than 2" << std::endl ;
        return 0 ;
    } else if ( global_choice_count_maximum > 20 )
    {
        log_out << "ERROR: Choice count maximum (" << global_choice_count_maximum << ") exceeds the specified array size limit of 20" << std::endl ;
        return 0 ;
    }


// -----------------------------------------------
//  Do all the tests for all the requested choice
//  counts.

    for ( global_specified_choice_count = global_choice_count_minimum ; global_specified_choice_count <= global_choice_count_maximum ; global_specified_choice_count ++ )
    {
        global_case_count_limit = global_maximum_case_count_per_choice_count ;
        do_all_tests_for_specified_choice_count( ) ;
    }


// -----------------------------------------------
//  Write the final results.

    write_final_results( ) ;


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
//  (c) Copyright 2021 by Richard Fobes at www.VoteFair.org.
//  You can redistribute and/or modify this VoteFairRanking software
//  under the MIT software license terms as explained in the
//  LICENSE file.)
//
//
// -----------------------------------------------
//
//  End of generate_random_ballots.cpp
