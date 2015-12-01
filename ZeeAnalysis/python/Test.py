import os
import sys
import numpy as np
from FunctionsRunGrid import *
import subprocess as sub

class node(object):
    def __init__(self, value, children = []):
        self.value = value
        self.children = children
        self.tag = ''
        self.runs = []

    def __repr__(self, level=0):
        ret = "\t"*level+repr(self.value)+':'+repr( self.tag )+"\n"
        for child in self.children:
            ret += child.__repr__(level+1)
        return ret

    def Insert( self, tagVect, run ) :
        if not len( tagVect ) : 
            self.runs.append( run )
            return 
        else :            
            index=self.FindSon( tagVect[-1] )
            self.children[index].Insert( tagVect[:-1], run )

    def FindSon( self, value ) :
        for child in self.children :
            if child.GetValue() == value : return self.children.index( child )
        self.children.append( node( value, [] ) )        
        return len( self.children) -1

    def GetValue( self ) :
        return self.value

    def GetRuns( self ) :
        return self.runs

    def CreateTag( self, endTag ) :
        self.tag = self.value + endTag
        for child in self.children :
            child.CreateTag( ('_' if self.value != '/' else '' ) + self.tag )
        pass
    
    def FillDatasets( self, datasets, doneRuns=[] ) :
        datasetGenName = 'data15_13TeV.*.physics_Main.merge.DAOD_EGAM1.*'
        if not len( self.children ) :
            foundRun=0
            dumDataset=[]
            for run in self.runs :
                if run in doneRuns :
                    foundRun=1
                else :
                    dumDataset.append( datasetGenName.replace( '*', '00'+ str( run ), 1 ) + self.tag)
                    doneRuns.append( run )

            datasets+=  [ datasetGenName + self.tag ] if not foundRun else dumDataset
            return
        else :
            for child in self.children : 
                child.FillDatasets( datasets, doneRuns )
                self.runs +=  child.GetRuns()
                pass

            nRuns = len( self.runs )
            self.runs = sorted( set( self.runs ), reverse=True ) 
            if len( self.runs ) != nRuns : 
                self.runs = []
                return 
            else :
                dumDatasets = [ datasetGenName + self.tag ]
                for d in datasets : 
                    if self.tag not in d : dumDatasets.append( d )
                datasets = dumDatasets


                
# tags=node( '/', []  )
# datasetList = np.genfromtxt( '/afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/DatasetList/Data_13TeV_Zee_25ns.csv', dtype='S100', delimiter=' ' )
# datasetList = sorted ( datasetList, reverse=True )
# for dataset in datasetList :
#     tags.Insert( dataset.split('.')[-1].split('/')[0].split('_') )
    
# tags.CreateTag('')    
# datasets=[]
# tags.FillRuns(datasets)
# #print tags
# for d in datasets : print d


    
# 
# foundRuns=[]
# totTag=''
# combTags = sorted( set( combTags ) )
# print tags

# while  len( datasetList ) != len( foundRuns ) :
#     testTag=''
#     for  nTag in tags :
        
#         for tag in nTag :
#             if testTag == '' : testTag = tag
            
#             pass

#     foundRuns.append( 1 )
