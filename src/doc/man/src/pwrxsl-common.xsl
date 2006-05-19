<?xml version='1.0'?>
<xsl:stylesheet  
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
    version="1.0">


<!-- Paths, graphics, extensions etc -->

<xsl:param name="use.extensions" select="'1'"></xsl:param>


<!-- Formatting options -->

<xsl:param name="emphasis.propagates.style" select="1"></xsl:param>
<xsl:param name="phrase.propagates.style" select="1"></xsl:param>
<xsl:param name="para.propagates.style" select="1"></xsl:param>

<xsl:param name="chapter.autolabel" select="1"></xsl:param> <!-- "1" could also be A, I, i, a for alphanumeric, roman etc.. -->
<xsl:param name="generate.section.toc.level" select="1"></xsl:param>
<xsl:param name="section.autolabel" select="1"/>
<xsl:param name="section.label.includes.component.label" select="1"/>
<xsl:param name="section.autolabel.max.depth" select="2"></xsl:param>

<xsl:param name="menuchoice.menu.separator"> → </xsl:param>
<xsl:param name="menuchoice.separator" select="'+'"></xsl:param>


<!-- tweak to change classname from monospace to regular. This overrides the template in inline.xsl, where inline.monoseq is called instead.  -->
<xsl:template match="classname">
  <xsl:call-template name="inline.charseq"/>
</xsl:template>


</xsl:stylesheet>