#ifndef ALI_TOKENIZER_H
#define ALI_TOKENIZER_H
#include <string>
#include "ali_tokenizer_define.h"

namespace ws
{

	class SegResult
	{

	public:
		/*
		 * Iterator on SegResult
		 */
		class Iterator
		{
		public:
			Iterator(SegResult* pSegResult, uint8_t iteratorType);
			~Iterator();
			Iterator(const Iterator& iter);
			Iterator& operator=(const Iterator& iter);
			bool operator==(const Iterator& iter)const;
			bool operator!=(const Iterator& iter)const;
			SegToken& operator*();
			SegToken* operator->();
			SegToken& operator++();
			SegToken& operator--();
			class IteratorImp* pIteratorImp;
		};

	public:

		virtual ~SegResult() = 0;

		/**
		 * get first token on main list or accessory list
		 * @param listType  MAIN_LIST means main list , ACCESSORY_LIST means accesory list
		 * @return the first token or NULL if the list is empty
		 */
		virtual SegToken* GetFirstToken(uint8_t listType) = 0;

		/**
		 * get last token on main list or accessory list
		 * @param listType MAIN_LIST means main list , ACCSSORY_LIST means accesory list
		 * @return the last token or NULL if the list is empty
		 */
		virtual SegToken* GetLastToken(uint8_t listType) = 0;

		/**
		 * get Iterator at the beginning
		 * @param iteratorType SEMANTIC_ITERATOR - visit semantic tokens
		 *                     RETRIEVE_ITERATOR- visit retrieval tokens
		 * @return Iterator
		 */
		virtual Iterator Begin(uint8_t iteratorType);

		/**
		 * get Iterator at the end
		 * @param iteratorType SEMANTIC_ITERATOR - visit semantic tokens
		 *                     RETRIEVAL_ITERATOR- visit retrieval tokens
		 * @return Iterator
		 */
		virtual Iterator End(uint8_t iteratorType);

		/**
		 * transform segmentaion result on main list into string
		 * @param listType MAIN_LIST means main list , ACCESSORY_LIST means accesory list
		 * @param outType 0 means output word(s) only
		 *                1 means output word(s) and rich information on word(s)
		 * @param output output segment result in
		 * @return true , on success
		 *         false on failure
		 */
		virtual bool ToString(uint8_t listType, uint8_t outType, std::string& output) = 0;

		/**
		* Get the POS tag name according to POS tag id
		* @param tagId POS tag id
		* @return pointer to POS tag name buffer, or NULL if fail.
		*/
		virtual const char* GetPosTagName(uint8_t posTagId) = 0;


		/**
		* Get the POS tag id according to POS tag name
		* @param tag POS tag name
		* @return id of tag, -1 means wrong tag name or POS model is not open.
		*/
		virtual int32_t GetPosTagId(const char* posTagName) = 0;

		/**
		* Get the semantic tag name according to semantic tag id, which be set in type_id.txt
		* @param semanticTagId semantic tag id
		* @return pointer to semantic tag name buffer, or NULL if fail.
		*/
		virtual const char* GetSemanticTagName(int16_t semanticTagId) = 0;

		/**
		* Get the semantic tag id according to semantic tag name, which be set in type_id.txt
		* @param semanticTagName semantic tag name
		* @return id of semantic tag, -1 means semantic tag name is wrong
		*/
		virtual int16_t GetSemanticTagId(const char* semanticTagName) = 0;

	};

	class AliTokenizer
	{
	public:

		virtual ~AliTokenizer() = 0;

		/**
		 * create a segmentation result
		 * @return a valid pointer to SegResult on success ,
		 *         NULL on failure
		 */
		virtual SegResult* CreateSegResult() = 0;

		/**
		 * release segmentation result, free memory allocated
		 * make sure pSegResult will longer in use after this call
		 * @param pSegResult SegResult to release [IN]
		 */
		virtual void ReleaseSegResult(SegResult* pSegResult) = 0;

		/**
		 * segment text , store result in pSegResult
		 * support UTF8/UTF16/GBK encoded text
		 * @param text text to be segmented  [IN]
		 * @param len length of text  [IN]
		 * @param enc encoding of text  [IN]
		 * @param pSegResult pointer of SegResult [OUT]
		 * @param tokenType bit-wise mask to determine which types of tokens should be output,
		 *        SEG_TOKEN_SEMANTIC - 输出语义单元
		 *        SEG_TOKEN_SEMANTIC|SEG_TOKEN_RETRIEVE - 输出语义单元和检索单元
		 * @return state of segmentation
		 *          0                   - success
		 *         -1                   - segementer not initialized
		 *         -2                   - invalid parameter such as NULL pointer
		 *         -3                   - unsupported encoding of text
		 *         or other failure code < 0
		 */
		virtual int32_t Segment(const char* text, uint32_t len, EnumEncodingSupported enc,
			SegTokenType tokenType, SegResult* pSegResult) = 0;

	};

	class AliTokenizerFactory
	{
	public:
		AliTokenizerFactory();
		~AliTokenizerFactory();

		/**
		* initialize segmenter(s) according to config file (eg. AliTokenizer.conf)
		* @param cfgFile configuration file path
		* @return true if loading successfully, false if fail
		*/
		bool Init(const char* cfgFile);

		/**
		 * get AliTokenizer instance according to tokenizerId
		 * @param tokenizerId tokenizer id in config file , eg. TAOBAO_CHINESE
		 * @return a initialized AliTokenizer instance on success
		 *         NULL on failure
		 */
		AliTokenizer* GetAliTokenizer(const char* tokenizerId);

		std::string Version() const;

	private:
		class AliTokenizerFactoryImpl* mImpl;
	};

}//namespace ws

#endif
