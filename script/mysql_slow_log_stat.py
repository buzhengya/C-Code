#!/usr/bin/env python
# encoding: utf-8
################################################################
#edit by vim
################################################################

import os
import sys
from subprocess import check_output

reload(sys)
sys.setdefaultencoding('utf8')

# save tables that appera
listTables = []

# save slow log record 
listSlowLog = []

#top slow log count
TOP_LOG_CNT = 50


#==================== USAGE =================
Usage = """============= usage ==============
python stat.py slowlogfile cnt		统计每张表出现的次数和平均耗时
python stat.py slowlogfile top		统计慢查询最慢50条MySql语句"""

class OneSlowLog :
	def __init__(self) :
		self.fTime = 0.0
		self.strTable = ""
		self.strStat = ""

def DealSlowFile(strFile) :
	#gen slow log
	GenSlowLog(strFile)
	
	#sort and print
	SortAndPrint()
	print "finish print top slow log."

def GenSlowLog(strFile) :
	f = open(strFile, 'r')
	slowLog = OneSlowLog()
	for line in f.readlines() :
		line = line.strip()
		#find # Time: one record begin...
		if line.find("# Time:") >= 0 :
			slowLog = OneSlowLog()
		if line.find("# Query_time:") >= 0:
			slowLog.fTime = float(GetCostTime(line))
			#print slowLog.fTime
		if line.find("tbl_") >= 0 :
			slowLog.strTable = GetTable(line)
			slowLog.strStat = line
			listSlowLog.append(slowLog)
			#print slowLog.strTable


def SortAndPrint() :
	listSlowLog.sort(cmp=None, key=lambda x:x.fTime, reverse = True)
	nEnd = TOP_LOG_CNT
	if nEnd > len(listSlowLog) :
		nEnd = len(listSlowLog)
	for i in range (nEnd) :
		slowLog = listSlowLog[i]
		print "cost time %f" % slowLog.fTime
		print slowLog.strStat
		print ""
			
def GetCostTime(strLine) :
	nSt = 0
	nEnd = 0
	for nIndex in range(len(strLine)) :
		c = strLine[nIndex]
		if c >= '0' and c <= '9' :
			nSt = nIndex 
			break
	
	for nIndex in range(nSt, len(strLine)) :
		c = strLine[nIndex]
		if (c >= '0' and c <= '9') or c == '.' :
			continue
		nEnd = nIndex
		break
	return strLine[nSt:nEnd]

def GetTable(strLine) :
	nSt = 0
	nEnd = 0
	nSt = strLine.find("tbl_")
	for nIndex in range(nSt, len(strLine)) :
		c = strLine[nIndex]
		if (c >= 'a' and c<= 'z') or c == '_' :
			continue
		nEnd = nIndex
		break
	return strLine[nSt:nEnd]
		
def SortByTable():
	listSlowLog.sort(cmp=None, key=lambda x:x.strTable, reverse=False)

def StatCntAndAvg(strFile):
	#gen slow log
	GenSlowLog(strFile)
	
	#sort slow log by table
	SortByTable()

	#print table cnt and avg
	PrintCntAndAvg()

	print "finish stat cnt and avg tm."

def PrintCntAndAvg() :
	strTable = ""
	nCnt = 0
	fTotalTm = 0.0
	for i in range(len(listSlowLog)) :
		slowLog = listSlowLog[i]
		if slowLog.strTable != strTable :
			if strTable != "" :
				print "table : %-30s.   count : %-5d.   avarage tm : %-2.1f.\n" % (strTable, nCnt, fTotalTm/nCnt)
			strTable = slowLog.strTable
			nCnt = 1
			fTotalTm = slowLog.fTime
		else :
			nCnt = nCnt + 1
			fTotalTm = fTotalTm + slowLog.fTime

	print "table : %-30s.   count : %-5d.   avarage tm : %-2.1f.\n" % (strTable, nCnt, fTotalTm/nCnt)

def GetTables() :
	strCmd = """grep -o "tbl_[a-z,_]*" mysql-slow.log | sort | uniq"""
	listInfo = os.popen(strCmd)

	for line in listInfo :
		strTable = line.strip('\n')
		#print strTable
		listTables.append(strTable)

def PrintOneKeyCnt(strKey, strFile) :
	strCmd = """grep "%s" %s | wc -l""" % (strKey, strFile)
	#print strCmd
	listInfo = os.popen(strCmd)
	strCnt = ""
	for line in listInfo :
		strCnt = line.strip('\n')
		break
	print "%s : %s" % (strKey, strCnt)

def PrintAllCnt(strFile) :
	GetTables()
	PrintOneKeyCnt("tbl_", strFile)
	for strTables in listTables :
		PrintOneKeyCnt(strTables, strFile)
	

def main() :
	if len(sys.argv) < 3 :
		print "please print mysql log file."
	strFile = sys.argv[1]
	strParam = sys.argv[2]
	if os.path.exists(strFile) == False :
		print "file %s not exist" % strFile
	
	if strParam == "cnt" :
		StatCntAndAvg(strFile)
	elif strParam == "top" :
		DealSlowFile(strFile)
	else :
		print "param error"


if __name__ == '__main__' :
	main()
