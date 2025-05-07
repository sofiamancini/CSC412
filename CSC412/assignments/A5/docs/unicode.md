# Part 3 Unicode

### 1. Who founded the Consortium? Who is represented among the current members, and how might that affect decisions being made for Unicode?
'The Unicode Consortium was incorporated in January, 1991 in the state of California, four years after the concept of a new character encoding, to be called "Unicode", was broached in discussions started by engineers from Xerox (Joe Becker) and Apple (Lee Collins and Mark Davis).'

'Members of the Consortium include major computer corporations, software producers, database vendors, government ministries, research institutions, international agencies, various user groups, and interested individuals. A white paper outlining the overall value of a Unicode membership to an organization is available separately.'

source: https://www.unicode.org/consortium/consort.html

The Unicode Consortium began in the 1980s with peoples from Xerox and Apple looking to incorporate Chinese and Japanese characters into computer languages. 

The Consortium is largely made up of companies, and as such the decisions being made are likely to stem from economic benefits to these members, instead of decisions made on behalf of improvements in the global community. The groups that are likely to receive the largest benefit from policies from the Consortium are those countries who are the largest tech producers and consumers, leaving other groups potentially underrepresented. 

### 2. Find a language that is not yet in Unicode, has only recently been supported (e.g., in the last 10 years), or is/was missing important characters. What is/was missing, and how does this affect the users of the language?

Indingenous communities in Canada identified 12 missing syllabic characters from the Unicode standard in 2022. From the article published by Unicode, the Nattilik community was unable to communicate reliably in their native language for simple text or email exchanges. By working closely with tribe leaders and elders, improvements were made such that this language, and many other indingenous languages can be used in modern forms of digital communication. Many native languages are becoming extinct because there are no new speakers, by introducing a viable form of communication for younger generations, Unicode could potentially support the preservation of local culture.

source: https://blog.unicode.org/2022/06/working-with-local-communities-to.html

### 3. For this question, you will need to work with a classmate! Make sure to coordinate so that you outline opposing positions for Part A (e.g. if you outline the ‘for’ position, your partner defends the ‘against’ position). Then, come together and discuss Part B!

Classmate: Gina Vincent


#### Step A: Outline either the position for or against Han Unification

Gina:
I’m outlining the argument **against** Han Unification. Han Unification is when Unicode merged similar-looking characters from Chinese, Japanese, and Korean into single code points because they share the same historical origin and meaning. The problem with this is that, even though the characters may look similar, they’re written differently depending on the language. For example, a Japanese user might expect a character to look one way, while a Chinese user would expect it to look slightly different. This can make digital text feel unfamiliar, awkward, or even incorrect. Opponents of unification argue that it ignores cultural and visual differences, making reading harder and misrepresenting how languages are written.  
(*Source: https://en.wikipedia.org/wiki/Han_unification*)

Sofia:
I will be aruging in favour of Han Unification. While there may be stylistic differences between Hanzi, Kanji, and Hanja, the characters all share a common origin and are mostly the same across each language, which justify them being treated as a single set in computer programming. Many of the stylistic changes we see are in relation to different fonts, and not a structural change of the character itself. For a unified font system used by a computer, these changes do not need to be accounted for. Additionally, when typing in these languages, users are typing the words phonetically, not though the characters. For example, in Japanese, a user would type 'としょかん' on their keyboard, and the resulting word would appear as '図書館'. This means that the intended meaning is clear to the user for each character that is used.


#### Step B
### A bullet-point summary of your discussion for Step B

Gina:
- My classmate argued **for** Han Unification, saying it made sense technically because the characters are largely the same across these languages.
- I argued **against** it, pointing out that it can erase important cultural differences in how characters are written and displayed.
- We agreed that modern fonts and systems usually handle this well now, but it's still frustrating when the display is wrong or unexpected.
- We both understood that the decision was made mostly to simplify digital encoding, but it comes at the cost of cultural accuracy and readability.

#### Describe the tradeoff that Unicode has made with this decision 

The Unicode Consortium chose to combine similar-looking Chinese, Japanese, and Korean characters into single code points so they could save space and keep the Unicode Standard manageable. This makes things easier for software developers because they don’t have to support thousands of extra characters. But the tradeoff is that sometimes, depending on your language settings or fonts, the character might appear in a style that doesn’t match your language, which can feel visually or culturally wrong.
