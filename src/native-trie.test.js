import * as trie from './native-trie'

describe('match', () => {

	test('single entry', () => {
		const t = new trie.StringTrie()
		t.insert('string', 'value')
		expect(t.match('string')).toEqual(['value'])
	})

	test('empty', () => {
		const t = new trie.StringTrie()
		expect(t.match('string')).toEqual([])
	})

	test('no match - long', () => {
		const t = new trie.StringTrie()
		t.insert('string', 'value')
		expect(t.match('string2')).toEqual([])
	})

	test('no match - incomplete', () => {
		const t = new trie.StringTrie()
		t.insert('string', 'value')
		expect(t.match('str')).toEqual([])
	})

	test('no match - different', () => {
		const t = new trie.StringTrie()
		t.insert('string', 'value')
		expect(t.match('grtsadda')).toEqual([])
	})

	test('many entries', () => {
		const t = new trie.StringTrie()
		t.insert('with', '1')
		t.insert('these', '2')
		t.insert('expectation', '3')
		t.insert('objects', '4')
		t.insert('except', '5')
		t.insert('call', '6')
		t.insert('matchers', '7')
		t.insert('on', '8')
		t.insert('them.', '9')
		t.insert('In', '10')
		t.insert('this', '11')
		t.insert('code', '12')
		t.insert('.toBe(4)', '13')
		t.insert('is', '14')
		t.insert('the', '15')
		t.insert('matcher.', '16')
		t.insert('When', '17')
		t.insert('Jest', '18')
		t.insert('runs,', '19')
		t.insert('it', '20')
		t.insert('tracks', '21')
		t.insert('all', '22')
		t.insert('the', '23')
		t.insert('failing', '24')
		t.insert('matchers', '25')
		t.insert('so', '26')
		t.insert('that', '27')
		t.insert('it', '28')
		t.insert('can', '29')
		t.insert('print', '30')
		t.insert('out', '31')
		t.insert('nice', '32')
		t.insert('error', '33')
		t.insert('messages', '34')
		t.insert('for', '35')
		t.insert('you', '36')
		expect(t.match('it')).toEqual(['20', '28'])
		expect(t.match('you')).toEqual(['36'])
		expect(t.match('code')).toEqual(['12'])
		expect(t.match('.toBe(4)')).toEqual(['13'])
		expect(t.match('sothat')).toEqual([])
	})

	test('unicode', () => {
		const t = new trie.StringTrie()
		t.insert('normal', 'ascii')
		t.insert('༚', 'Tibetan Sign Rdel Dkar Gcig')
		expect(t.match('༚')).toEqual(['Tibetan Sign Rdel Dkar Gcig'])
		expect(t.match('normal')).toEqual(['ascii'])
	})

	test('unicode mixed', () => {
		const t = new trie.StringTrie()
		t.insert('normal', 'ascii')
		t.insert('༚႞႞႞aa', 'unicode ascii')
		t.insert('༚႞႞႞aa', 'unicode')
		t.insert('ba༚႞႞႞aa', 'ascii unicode')
		t.insert('ba༚႞႞႞aa', 'unicode 2')
		t.insert('ba', 'ascii 2')
		expect(t.match('༚႞႞႞aa')).toEqual(['unicode ascii', 'unicode'])
		expect(t.match('ba༚႞႞႞aa')).toEqual(['ascii unicode', 'unicode 2'])
		expect(t.match('normal')).toEqual(['ascii'])
		expect(t.match('ba')).toEqual(['ascii 2'])
	})

	test('unicode combining diacrititcs', () => {
		// The ambigous symbols like 'é' ('\u00E9') and 'é' ('\u0065\u0301') should not be matched by this trie
		// Instead they should be normalized before being added
		const t = new trie.StringTrie()
		t.insert('e', 'character e')
		t.insert('é', 'character é')
		t.insert('e\u0301', 'e and combining accent')
		t.insert('é', 'e and combining accent 2')
		expect(t.match('é')).toEqual(['e and combining accent', 'e and combining accent 2'])
		expect(t.match('é')).toEqual(['character é'])
		expect(t.match('e')).toEqual(['character e'])
	})

})

describe('matchPartial', () => {

	test('empty', () => {
		const t = new trie.StringTrie()
		expect(t.matchPartial('string')).toEqual({})
	})

	test('no match - long', () => {
		const t = new trie.StringTrie()
		t.insert('string', 'value')
		expect(t.matchPartial('string2')).toEqual({})
	})

	test('match - incomplete', () => {
		const t = new trie.StringTrie()
		t.insert('string', 'value')
		expect(t.matchPartial('str')).toEqual({3:['value']})
	})

	test('no match - different', () => {
		const t = new trie.StringTrie()
		t.insert('string', 'value')
		expect(t.matchPartial('grtsadda')).toEqual({})
	})

	test('multiple partial match', () => {
		const t = new trie.StringTrie()
		t.insert('abcd', 'value 1')
		t.insert('abcd', 'value 2')
		expect(t.matchPartial('a')).toEqual({3:['value 1', 'value 2']})
	})

	test('multiple level partial match', () => {
		const t = new trie.StringTrie()
		t.insert('abcd', 'value 1')
		t.insert('abcde', 'value 2')
		expect(t.matchPartial('a')).toEqual({3:['value 1'], 4:['value 2']})
	})

	test('complex match', () => {
		const t = new trie.StringTrie()
		t.insert('abcd', '1.1')
		t.insert('abcd', '1.2')
		t.insert('abcde', '2.1')
		t.insert('abcde', '2.2')
		t.insert('abcdefg', '4.1')
		t.insert('abcdefg', '4.2')
		t.insert('bcdef', '0')
		expect(t.matchPartial('abcd')).toEqual({0: ['1.1','1.2'], 1:['2.1','2.2'], 3:['4.1','4.2']})
	})

	test('constrained | no exact match', () => {
		const t = new trie.StringTrie()
		t.insert('abcd', '1.1')
		t.insert('abcd', '1.2')
		t.insert('abcde', '2.1')
		t.insert('abcde', '2.2')
		t.insert('abcdefg', '4.1')
		t.insert('abcdefg', '4.2')
		t.insert('bcdef', '0')
		expect(t.matchPartial('abcd', {includeExactMatch: false})).toEqual({1:['2.1','2.2'], 3:['4.1','4.2']})
	})

	test('constrained | limited depth', () => {
		const t = new trie.StringTrie()
		t.insert('abcd', '1.1')
		t.insert('abcd', '1.2')
		t.insert('abcde', '2.1')
		t.insert('abcde', '2.2')
		t.insert('abcdefg', '4.1')
		t.insert('abcdefg', '4.2')
		t.insert('bcdef', '0')
		expect(t.matchPartial('abcd', {maxDepth: 2})).toEqual({0:['1.1','1.2'], 1:['2.1','2.2']})
	})

	test('constrained | limited depth with empty', () => {
		const t = new trie.StringTrie()
		t.insert('abcd', '1.1')
		t.insert('abcd', '1.2')
		t.insert('abcde', '2.1')
		t.insert('abcde', '2.2')
		t.insert('abcdefg', '4.1')
		t.insert('abcdefg', '4.2')
		t.insert('bcdef', '0')
		expect(t.matchPartial('abcd', {includeExactMatch: false, maxDepth: -1, maxElements: 1})).toEqual({1:['2.1']})
	})

	test('constrained | limited number', () => {
		const t = new trie.StringTrie()
		t.insert('abcd', '1.1')
		t.insert('abcd', '1.2')
		t.insert('abcde', '2.1')
		t.insert('abcde', '2.2')
		t.insert('abcdefg', '4.1')
		t.insert('abcdefg', '4.2')
		t.insert('bcdef', '0')
		expect(t.matchPartial('abcd', {maxElements: 3})).toEqual({0:['1.1','1.2'], 1:['2.1']})
	})

})
