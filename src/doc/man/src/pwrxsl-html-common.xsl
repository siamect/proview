<?xml version='1.0'?>
<xsl:stylesheet  
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
    version="1.0">

<!-- Set html-common parameters -->


<xsl:param name="img.src.path">../</xsl:param>

<xsl:param name="callout.graphics" select="1"></xsl:param>
<xsl:param name="callout.graphics.path">../</xsl:param>
<xsl:param name="callout.graphics.extension" select="'.png'"></xsl:param>

<xsl:param name="admon.graphics" select="1"/>
<xsl:param name="admon.graphics.path">../</xsl:param>
<xsl:param name="admon.graphics.extension" select="'.png'"/>

<xsl:param name="navig.graphics" select="1"></xsl:param>
<xsl:param name="navig.graphics.extension" select="'.gif'"/>
<xsl:param name="navig.graphics.path">../</xsl:param>


<xsl:param name="html.stylesheet" select="'pwrdoc.css'"/> <!-- Define css for html files -->

<xsl:param name="keep.relative.image.uris" select="0"/>

<xsl:param name="preferred.mediaobject.role" select="'html'"/>

<xsl:template match="*" mode="admon.graphic.width">
  <xsl:text>32pt</xsl:text>
</xsl:template>


<!-- tweak to allow css pickup of pwrenv span class -->

<xsl:template match="application">
  <xsl:choose>
    <xsl:when test="@role='pwrenv'">
      <span>
	<xsl:attribute name="class">
	  <xsl:text>pwrenv</xsl:text>
	</xsl:attribute>
	<xsl:apply-templates/>
      </span>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="inline.charseq"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>
	


</xsl:stylesheet>