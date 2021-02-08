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
//  Version 1.21
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
//  number of ballots and the number of choices.

const int global_maximum_case_count = 10000 ;
const int global_maximum_ballot_number = 17 ;
const int global_maximum_choice_number = 7 ;


// -----------------------------------------------
//  (Do not change these numbers.)
//  Assign a number to each kind of test.

const int global_test_matches_with_votefair_ranking = 1 ;
const int global_test_irrelevant_alternatives = 2 ;
const int global_test_clone_independence = 3 ;


// -----------------------------------------------
//  Change this value to specify which test to run.

const int global_test_type = global_test_matches_with_votefair_ranking ;
// const int global_test_type = global_test_irrelevant_alternatives ;
// const int global_test_type = global_test_clone_independence ;


// -----------------------------------------------
//  Ensure that the case numbers have the same
//  number of digits.  This number can be changed.

const int global_minimum_case_id = 100000 ;


// -----------------------------------------------
//  Assign numbers and names to the vote-counting
//  methods, and specify how many methods there
//  are.

int global_number_of_methods = 6 ;
const int global_method_votefair = 1 ;
const int global_method_ipe = 2 ;
const int global_method_rcipe = 3 ;
const int global_method_ple = 4 ;
const int global_method_irv = 5 ;
const int global_method_star = 6 ;

std::string global_name_for_method_votefair = "VF" ;
std::string global_name_for_method_ipe = "IPE" ;
std::string global_name_for_method_rcipe = "RCIPE" ;
std::string global_name_for_method_ple = "PLE" ;
std::string global_name_for_method_irv = "IRV" ;
std::string global_name_for_method_star = "STAR" ;


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
int global_count_of_tests ;
int global_choice_count_case_specific ;
int global_choice_omitted ;
int global_count_of_votefair_single_winner ;
int global_count_of_votefair_no_single_winner ;


// -----------------------------------------------
//  Declare the needed arrays.

std::string global_name_for_method[ 20 ] ;

int global_choice_on_ballot_at_ranking_level[ 200 ][ 20 ] ;
int global_choice_number_at_position[ 99 ] ;
int global_usage_count_for_choice_and_rank[ 99 ][ 99 ] ;

int global_choice_winner_all_choices_for_method[ 20 ] ;
int global_count_of_cases_that_match_for_method[ 20 ] ;
int global_count_of_cases_that_fail_match_for_method[ 20 ] ;
int global_count_of_cases_tied_for_method[ 20 ] ;
int global_count_of_group_failures_for_method[ 20 ] ;
int global_count_of_group_ties_for_method[ 20 ] ;
int global_choice_winner_from_method[ 20 ] ;


// -----------------------------------------------
//  Specify codes that identify the meaning of the
//  next number in the coded output list.

std::string global_voteinfo_code_for_end_of_all_cases = "-2" ;
std::string global_voteinfo_code_for_case_number = "-3" ;
std::string global_voteinfo_code_for_question_number = "-4" ;
std::string global_voteinfo_code_for_number_of_choices = "-6" ;
std::string global_voteinfo_code_for_start_of_all_vote_info = "-7" ;
std::string global_voteinfo_code_for_end_of_all_vote_info = "-8" ;
std::string global_voteinfo_code_for_end_of_ballot = "-10" ;
std::string global_voteinfo_code_for_ballot_count = "-11" ;
std::string global_voteinfo_code_for_preference_level = "-12" ;

std::string global_voteinfo_code_for_request_instant_runoff_voting = "-50" ;
std::string global_voteinfo_code_for_request_instant_pairwise_elimination = "-51" ;
std::string global_voteinfo_code_for_request_irv_minus_pairwise_losers = "-52" ;
std::string global_voteinfo_code_for_request_star_voting = "-56" ;
std::string global_voteinfo_code_for_request_pairwise_loser_elimination = "-58" ;

const int global_voteinfo_code_for_choice = -13 ;
const int global_voteinfo_code_for_tie = -14 ;
const int global_voteinfo_code_for_start_of_votefair_popularity_ranking_sequence_results = -15 ;

const int global_voteinfo_code_for_winner_instant_runoff_voting = -53 ;
const int global_voteinfo_code_for_winner_instant_pairwise_elimination = -54 ;
const int global_voteinfo_code_for_winner_irv_minus_pairwise_losers = -55 ;
const int global_voteinfo_code_for_winner_star_voting = -57 ;
const int global_voteinfo_code_for_winner_pairwise_loser_elimination = -59 ;


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

    int ballot_number = 0 ;
    int choice_number = 0 ;
    int ranking_level = 0 ;
    int position_number = 0 ;
    int pointer_number = 0 ;
    int count_for_ranking_level = 0 ;
    int count_of_choices_not_yet_ranked = 0 ;
    int count_of_choices_need_to_rank = 0 ;

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
//  If clone independence is being tested, hide
//  the last choice, which will be inserted
//  immediately after choice one.

        count_of_choices_need_to_rank = global_maximum_choice_number ;
        if ( ( global_test_type == global_test_clone_independence ) && ( global_choice_omitted > 0 ) )
        {
            count_of_choices_need_to_rank = global_maximum_choice_number - 1 ;
        }


// -----------------------------------------------
//  Begin a loop that handles one ranking level.

        ranking_level = 1 ;
        count_of_choices_not_yet_ranked = count_of_choices_need_to_rank ;
        for ( count_for_ranking_level = 1 ; count_for_ranking_level <= count_of_choices_need_to_rank ; count_for_ranking_level ++ )
        {


// -----------------------------------------------
//  Randomly choose a choice number for this
//  ranking level.  Do not repeat any choice number
//  already used at a previous ranking level.

            std::uniform_int_distribution<int> distribution( 1 , count_of_choices_not_yet_ranked );
            position_number = distribution( generator );
            choice_number = global_choice_number_at_position[ position_number ] ;
            global_choice_on_ballot_at_ranking_level[ ballot_number ][ ranking_level ] = choice_number ;


// -----------------------------------------------
//  If clone independence is being tested, and a
//  ranking level was just assigned to choice one,
//  insert the clone immediately below choice one.

            if ( ( global_test_type == global_test_clone_independence ) && ( choice_number == 1 ) )
            {
                ranking_level ++ ;
                global_choice_on_ballot_at_ranking_level[ ballot_number ][ ranking_level ] = global_maximum_choice_number ;
            }


// -----------------------------------------------
//  Update the list of which choices still need to
//  be assigned to a ranking level.

            for ( pointer_number = position_number ; pointer_number <= count_of_choices_not_yet_ranked - 1 ; pointer_number ++ )
            {
                global_choice_number_at_position[ pointer_number ] = global_choice_number_at_position[ pointer_number + 1 ] ;
            }


// -----------------------------------------------
//  Keep track of choice number usage to allow
//  verifying randomness.

            global_usage_count_for_choice_and_rank[ choice_number ][ ranking_level ] ++ ;


// -----------------------------------------------
//  Repeat the loop that handles one ranking level.

            count_of_choices_not_yet_ranked -- ;
            ranking_level ++ ;
        }


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

    int method_id = 1 ;
    std::string input_line ;
    std::string input_text_word ;


// -----------------------------------------------
//  Initialization.

    int current_result_code = 0 ;
    int previous_result_code = 0 ;
    int next_result_code = 0 ;
    int count_of_result_codes = 0 ;
    int count_position_at_start_of_votefair_popularity_sequence = 0 ;
    int count_position_at_choice_number = 0 ;
    int choice_number_adjustment = 0 ;
    for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
    {
        global_choice_winner_from_method[ method_id ] = 0 ;
    }


// -----------------------------------------------
//  Begin loop to handle one line from the input file
//  that contains the calculated results.

    log_out << std::endl << "[" << global_case_id << "]" << "[ch " << global_choice_count_case_specific << "]" ;
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

            if ( current_result_code == global_voteinfo_code_for_start_of_votefair_popularity_ranking_sequence_results )
            {
                count_position_at_start_of_votefair_popularity_sequence = count_of_result_codes ;

            } else if ( current_result_code == global_voteinfo_code_for_choice )
            {
                count_position_at_choice_number = count_of_result_codes ;

            } else if ( ( count_of_result_codes == count_position_at_start_of_votefair_popularity_sequence + 2 ) && ( count_of_result_codes == count_position_at_choice_number + 1 ) )
            {
                for ( method_id = 2 ; method_id <= global_number_of_methods ; method_id ++ )
                {
                    global_choice_winner_from_method[ method_id ] = 0 ;
                }
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

            } else if ( previous_result_code == global_voteinfo_code_for_winner_star_voting )
            {
                global_choice_winner_from_method[ global_method_star ] = current_result_code ;
                log_out << "[" << global_name_for_method[ global_method_star ] << " " << global_choice_winner_from_method[ global_method_star ] << "]" ;

            } else if ( previous_result_code == global_voteinfo_code_for_winner_pairwise_loser_elimination )
            {
                global_choice_winner_from_method[ global_method_ple ] = current_result_code ;
                log_out << "[" << global_name_for_method[ global_method_ple ] << " " << global_choice_winner_from_method[ global_method_ple ] << "]" ;

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
//  If testing for matches against VoteFair
//  ranking, count the number of cases that match,
//  or fail to match, the winner according to
//  VoteFair popularity ranking.
//  Also count ties, but do not count a tie as
//  either a match or a failure to match.
//  If the tie occurs in VoteFair popularity
//  ranking, the case is ignored.

    if ( global_test_type == global_test_matches_with_votefair_ranking )
    {
        if ( global_choice_winner_from_method[ global_method_votefair ] > 0 )
        {
            global_count_of_votefair_single_winner ++ ;
            for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
            {
                if ( global_choice_winner_from_method[ method_id ] == global_choice_winner_from_method[ global_method_votefair ] )
                {
                    global_count_of_cases_that_match_for_method[ method_id ] ++ ;
                } else if ( global_choice_winner_from_method[ method_id ] > 0 )
                {
                    global_count_of_cases_that_fail_match_for_method[ method_id ] ++ ;
                } else
                {
                    global_count_of_cases_tied_for_method[ method_id ] ++ ;
                }
            }
        } else
        {
            global_count_of_votefair_no_single_winner ++ ;
        }
    }


// -----------------------------------------------
//  Identify interesting cases:

    if ( ( global_choice_winner_from_method[ global_method_ple ] < 0 ) && ( global_choice_winner_from_method[ global_method_rcipe ] > 0 ) && ( global_choice_winner_from_method[ global_method_ipe ] > 0 ) && ( global_choice_winner_from_method[ global_method_irv ] > 0 ) && ( global_choice_winner_from_method[ global_method_votefair ] == global_choice_winner_from_method[ global_method_rcipe ] ) && ( global_choice_winner_from_method[ global_method_rcipe ] != global_choice_winner_from_method[ global_method_irv ] ) )
    {
        log_out << "[****]" ;
    }


// -----------------------------------------------
//  If this case included all the choices, save
//  the winner based on each method.

    if ( global_choice_count_case_specific == global_maximum_choice_number )
    {
        global_choice_winner_all_choices_for_method[ global_method_votefair ] = global_choice_winner_from_method[ global_method_votefair ] ;
        for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
        {
            global_choice_winner_all_choices_for_method[ method_id ] = global_choice_winner_from_method[ method_id ] ;
        }
    }


// -----------------------------------------------
//  If testing independence of irrelevant
//  alternatives or clone independence, determine
//  whether the winner is the same as the case
//  with all the choices.  If not, and if the
//  omitted choice is a non-winning choice, count
//  it as a failure for the counting method.

    if ( ( ( global_test_type == global_test_irrelevant_alternatives ) || ( global_test_type == global_test_clone_independence ) ) && ( global_choice_count_case_specific < global_maximum_choice_number ) )
    {
        for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
        {
            if ( ( global_choice_winner_from_method[ method_id ] > 0 ) && ( global_choice_winner_all_choices_for_method[ method_id ] > 0 ) )
            {
                if ( global_choice_omitted != global_choice_winner_all_choices_for_method[ method_id ] )
                {
                    choice_number_adjustment = 0 ;
                    if ( global_choice_winner_all_choices_for_method[ method_id ] > global_choice_omitted )
                    {
                        choice_number_adjustment = 1 ;
                    }
                    if ( global_choice_winner_from_method[ method_id ] + choice_number_adjustment != global_choice_winner_all_choices_for_method[ method_id ] )
                    {
                        global_count_of_group_failures_for_method[ method_id ] ++ ;
                        log_out << "[" << global_name_for_method[ method_id ] << "_fail]" ;
                    }
                }
            } else
            {
                global_count_of_group_ties_for_method[ method_id ] ++ ;
//                log_out << "[" << global_name_for_method[ method_id ] << "_tie]" ;
            }
        }

        if ( global_choice_omitted == global_maximum_choice_number )
        {
            for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
            {
                if ( global_count_of_group_failures_for_method[ method_id ] > 0 )
                {
                    global_count_of_cases_that_fail_match_for_method[ method_id ] ++ ;
                }
            }
        }
    }


// -----------------------------------------------
//  End of function handle_calculated_results

    return ;

}


// -----------------------------------------------
// -----------------------------------------------
//  Execution starts here.

int main( ) {


// -----------------------------------------------
//  Declare integer variables.

    int method_id = 1 ;
    int case_count = 0 ;
    int ballot_number = 0 ;
    int choice_number = 0 ;
    int ranking_level = 0 ;
    int position_number = 0 ;
    int pointer_number = 0 ;
    int count_of_choices_not_yet_ranked = 0 ;
    int integer_from_system_call = 0 ;
    int calculated_result_agree = 0 ;
    int calculated_result_disagree = 0 ;
    int calculated_result_tied = 0 ;
    int calculated_result_failures = 0 ;
    int count_of_cases_agree_plus_disagree = 0 ;
    int calculated_result_ties = 0 ;


// -----------------------------------------------
//  Initialization.

    global_case_id = 0 ;
    global_count_of_tests = 0 ;
    global_choice_count_case_specific = 0 ;
    global_choice_omitted = 0 ;
    global_count_of_votefair_single_winner = 0 ;
    global_count_of_votefair_no_single_winner = 0 ;
    global_choice_count_case_specific = global_maximum_choice_number ;
    global_name_for_method[ global_method_votefair ] = global_name_for_method_votefair ;
    global_name_for_method[ global_method_ipe ] = global_name_for_method_ipe ;
    global_name_for_method[ global_method_rcipe ] = global_name_for_method_rcipe ;
    global_name_for_method[ global_method_star ] = global_name_for_method_star ;
    global_name_for_method[ global_method_irv ] = global_name_for_method_irv ;
    global_name_for_method[ global_method_ple ] = global_name_for_method_ple ;
    for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
    {
        global_count_of_group_ties_for_method[ method_id ] = 0 ;
    }


// -----------------------------------------------
//  Open the output log file.

    log_out.open ( "temp_log_from_generate_random_ballots.txt" , std::ios::out ) ;


// -----------------------------------------------
//  Begin a loop that handles one case.

    global_case_id = global_minimum_case_id ;
    global_choice_omitted = global_maximum_choice_number + 99 ;
    for ( case_count = 1 ; case_count <= global_maximum_case_count ; case_count ++ )
    {
        std::cout << "." ;


// -----------------------------------------------
//  When testing clone independence, update the
//  global_choice_omitted pointer.  It equals zero
//  when all choices are used, otherwise it equals
//  the full number of choices.

        if ( global_test_type == global_test_clone_independence )
        {
            if ( global_choice_omitted > global_maximum_choice_number )
            {
                global_choice_omitted = 0 ;
                global_count_of_tests ++ ;
                for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
                {
                    global_count_of_group_failures_for_method[ method_id ] = 0 ;
                }
            } else
            {
                global_choice_omitted = global_maximum_choice_number ;
            }
        }


// -----------------------------------------------
//  When needed, reset the global_choice_omitted
//  pointer when it has cycled through all the
//  choices.  It equals zero when all choices are
//  used, and otherwise it equals the choice to omit.
//  Also reset the group counts.
//  And count the number of tests, which is less
//  than the number of cases if testing IIA or
//  clone independence.

        if ( global_choice_omitted > global_maximum_choice_number )
        {
            global_choice_omitted = 0 ;
            global_count_of_tests ++ ;
            for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
            {
                global_count_of_group_failures_for_method[ method_id ] = 0 ;
            }
        }


// -----------------------------------------------
//  If  one of the choices is being omitted,
//  specify one less than the usual number of
//  choices.

        global_choice_count_case_specific = global_maximum_choice_number ;
        if ( ( global_test_type != global_test_matches_with_votefair_ranking ) && ( global_choice_omitted > 0 ) )
        {
            global_choice_count_case_specific = global_maximum_choice_number - 1 ;
        }


// -----------------------------------------------
//  When needed, randomly generate new ballot
//  rankings.

        if ( global_choice_count_case_specific == global_maximum_choice_number )
        {
            generate_preferences( ) ;
        }


// -----------------------------------------------
//  Write the beginning of a new output file.

        std::fstream outfile ;
        outfile.open ( "temp_generated_random_ballots.txt" , std::fstream::out ) ;
        outfile << global_voteinfo_code_for_start_of_all_vote_info << std::endl ;
        outfile << global_voteinfo_code_for_case_number << " " << global_case_id << std::endl ;
        outfile << global_voteinfo_code_for_request_instant_pairwise_elimination << std::endl ;
        outfile << global_voteinfo_code_for_request_instant_runoff_voting << std::endl ;
        outfile << global_voteinfo_code_for_request_irv_minus_pairwise_losers << std::endl ;
        outfile << global_voteinfo_code_for_request_star_voting << std::endl ;
        outfile << global_voteinfo_code_for_request_pairwise_loser_elimination << std::endl ;
        outfile << global_voteinfo_code_for_question_number << " " << global_question_number << std::endl ;
        outfile << global_voteinfo_code_for_number_of_choices << " " << global_choice_count_case_specific << std::endl ;


// -----------------------------------------------
//  Begin a loop that handles one ballot.
//  Write the beginning of the info for this ballot.

        for ( ballot_number = 1 ; ballot_number <= global_maximum_ballot_number ; ballot_number ++ )
        {
            outfile << global_voteinfo_code_for_ballot_count << " 1" << std::endl ;
            outfile << global_voteinfo_code_for_question_number << " " << global_question_number << std::endl ;


// -----------------------------------------------
//  Begin a loop that handles one ranking level.

            for ( ranking_level = 1 ; ranking_level <= global_choice_count_case_specific ; ranking_level ++ )
            {


// -----------------------------------------------
//  Write the choice number that is at the
//  next-lower preference level.  But not if it
//  is an omitted choice.  Also, adjust the choice
//  number if needed.

                choice_number = global_choice_on_ballot_at_ranking_level[ ballot_number ][ ranking_level ] ;
                if ( ( global_choice_count_case_specific != global_maximum_choice_number ) && ( choice_number > global_choice_omitted ) )
                {
                    choice_number -- ;
                }
                if ( ( global_choice_count_case_specific == global_maximum_choice_number ) || ( choice_number != global_choice_omitted ) )
                {
                    outfile << choice_number << std::endl ;
                }


// -----------------------------------------------
//  Repeat the loop that handles one preference ranking.

            }


// -----------------------------------------------
//  Write the code that terminates this ballot.

            outfile << global_voteinfo_code_for_end_of_ballot << std::endl ;


// -----------------------------------------------
//  Repeat the loop that handles one ballot.

        }


// -----------------------------------------------
//  Write the codes at the end of the output file.

        outfile << global_voteinfo_code_for_end_of_all_vote_info << std::endl ;
        outfile << global_voteinfo_code_for_end_of_all_cases << std::endl ;


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
//  Update the case number.  Also update the
//  choice omitted number in case it is being used.

        global_case_id ++ ;  
        global_choice_omitted ++ ;


// -----------------------------------------------
//  Repeat the loop that handles one case.

    }


// -----------------------------------------------
//  Begin to write the results.

    log_out << std::endl << std::endl << std::endl ;

    if ( global_test_type == global_test_matches_with_votefair_ranking )
    {
        log_out << "[test type: Matches with VoteFair ranking results]" << std::endl ;
    } else if ( global_test_type == global_test_irrelevant_alternatives )
    {
        log_out << "[test type: Independence of Irrelevant Alternatives (IIA)]" << std::endl ;
    } else if ( global_test_type == global_test_clone_independence )
    {
        log_out << "[test type: Clone Independence]" << std::endl ;
    } else
    {
        log_out << "[error: test type invalid]" << std::endl ;
    }

    if ( global_test_type == global_test_matches_with_votefair_ranking )
    {
        log_out << "[number of cases attempted: " << global_maximum_case_count << "]" << std::endl ;
    } else
    {
        log_out << "[number of cases: " << global_count_of_tests << "]" << std::endl ;
    }

    log_out << "[number of ballots: " << global_maximum_ballot_number << "]" << std::endl ;
    log_out << "[number of choices: " << global_maximum_choice_number << "]" << std::endl ;
    log_out << "[note: " << '"' << "per k" << '"' << " means per thousand, so divide by 10 to get percentage]" << std::endl ;


// -----------------------------------------------
//  If testing comparisons with VoteFair ranking,
//  calculate and write those results.

    if ( global_test_type == global_test_matches_with_votefair_ranking )
    {
        for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
        {
            count_of_cases_agree_plus_disagree = global_count_of_cases_that_match_for_method[ method_id ] + global_count_of_cases_that_fail_match_for_method[ method_id ] ;
            if ( count_of_cases_agree_plus_disagree > 0 )
            {
                calculated_result_agree = int( ( 1000 * global_count_of_cases_that_match_for_method[ method_id ] ) / count_of_cases_agree_plus_disagree ) ;
                calculated_result_disagree = int( ( 1000 * global_count_of_cases_that_fail_match_for_method[ method_id ] ) / count_of_cases_agree_plus_disagree ) ;
                calculated_result_tied = global_count_of_cases_tied_for_method[ method_id ] ;
                log_out << "[" << global_name_for_method[ method_id ] << " agree/disagree per k: " << calculated_result_agree << "  " << calculated_result_disagree << "  (" << calculated_result_tied << " ties)]" << std::endl ;
            } else
            {
                log_out << "[" << global_name_for_method[ method_id ] << " has zero case count]" << std::endl ;
            }
        }
    }


// -----------------------------------------------
//  If testing independence of irrelevant
//  alternatives, or clone independence, calculate
//  and write those results.

    if ( ( global_test_type == global_test_irrelevant_alternatives ) || ( global_test_type == global_test_clone_independence ) )
    {
        for ( method_id = 1 ; method_id <= global_number_of_methods ; method_id ++ )
        {
            calculated_result_failures = int( ( 1000 *  global_count_of_cases_that_fail_match_for_method[ method_id ] ) / global_count_of_tests ) ;
            calculated_result_ties = int( ( 1000 *  global_count_of_group_ties_for_method[ method_id ] ) / global_count_of_tests ) ;
            log_out << "[" << global_name_for_method[ method_id ] << " failures per k: " << calculated_result_failures << " (ties per k: " << calculated_result_ties << ")]" << std::endl ;
        }
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
//  (c) Copyright 2021 by Richard Fobes at www.VoteFair.org.
//  You can redistribute and/or modify this VoteFairRanking software
//  under the MIT software license terms as explained in the
//  LICENSE file.)
//
//
// -----------------------------------------------
//
//  End of generate_random_ballots.cpp
