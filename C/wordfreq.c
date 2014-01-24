/*
 * This program prints the twenty most frequent words in a text file.
 *
 *
 * Run on Mark Twain's Huckleberry Finn, the top twenty words
 * are:
 * and     6255
 * the     4909
 * to      2987
 * a       2982
 * i       2593
 * it      2128
 * was     2033
 * of      1763
 * in      1485
 * he      1464
 * you     1217
 * that    1025
 * on      877
 * for     864
 * up      830
 * all     816
 * but     811
 * so      798
 * out     793
 * we      728
 *
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXWORDS	10000
#define MAXSTRING	100

/* structure holding word frequency information */

typedef struct _word {
	char	s[MAXSTRING];	/* the word */
	int	count;		/* number of times word occurs */
} word;

/* 
 * This function inserts a word into the list of words.  If the word is
 * not yet in the list, a new slot is used with the count set to 1.
 * Otherwise, the count is simply incremented.
 */
void insert_word (word *words, int *n, char *s) {
	int	i;

	/* linear search for the word */
	for (i=0; i<*n; i++) if (strcmp (s, words[i].s) == 0) {

		/* found it?  increment and return. */

		words[i].count++;
		return;
	}

	/* error conditions... */

	if (strlen (s) >= MAXSTRING) {
		fprintf (stderr, "word too long!\n");
		exit (1);
	}
	if (*n >= MAXWORDS) {	
		fprintf (stderr, "too many words!\n");
		exit (1);
	}

	/* copy the word into the structure at the first available slot,
	 * i.e., *n
	 */

	strcpy (words[*n].s, s);

	/* this word has occured once up to now, so count = 1 */

	words[*n].count = 1;

	/* one more word */

	(*n)++;
}

/* comparison function for quicksort.  this lets quicksort sort words
 * by descending order of count, i.e., from most to least frequent
 */
int wordcmp (word *a, word *b) {
	if (a->count < b->count) return +1;
	if (a->count > b->count) return -1;
	return 0;
}

/* return 1 if c is alphabetic (a..z or A..Z), 0 otherwise */
int is_alpha (char c) {
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') return 1;
	return 0;
}

/* remove the i'th character from the string s */
void remove_char (char *s, int i) {
	while (s[i]) {
		i++;
		s[i-1] = s[i];
	}
	s[i] = 0;
}

/* remove non-alphabetic characters from the string s */
void remove_non_alpha (char *s) {
	int	i;

	for (i=0; s[i]; i++) if (!is_alpha (s[i])) remove_char (s, i);
}

/* make all the letters in s lowercase */
void make_lowercase (char *s) {
	int	i;

	for (i=0; s[i]; i++) s[i] = tolower (s[i]);
}

/* main program */
int main () {
	word 	words[MAXWORDS];
	char	s[1000];
	int	i, n, m;

	n = 0;

	/* read all the words in the file... */

	while (!feof (stdin)) {
		scanf ("%s", s);

		/* only insert the word if it's not punctuation */

		if (is_alpha (s[0])) {

			/* get rid of non-letters */

			remove_non_alpha (s);

			/* make all letters lowercase */

			make_lowercase (s);

			/* put this word in the list */

			insert_word (words, &n, s);
		}
	}

	/* sort the list of words by descending frequency */

	qsort((void *) words, n, sizeof (word),
		(int (*) (const void *, const void *)) wordcmp);

	
	/* if fewer than 20 words in total, just print up the the
	 * first n words
	 */
	if (n < 20) 
		m = n;
	else
		m = 20;

	/* print the words with their frequencies */

	for (i=0; i<m; i++)
		printf ("%s\t%d\n", words[i].s, words[i].count);
}
