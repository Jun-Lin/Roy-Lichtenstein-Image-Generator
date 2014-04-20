#include <stdio.h>
#include "dib_Fonts.h"

/* glyph data record */
typedef struct {
   int width;
   int height;
   unsigned char *name;
}glyph;

/*
 * XBM font data
 */
#include "FreeMono_Bold10_font.xbm"
/* array of glyphs */
static glyph FreeMono_Bold10_glyphsArray[] = {
   {FreeMono_Bold10_ascii_33fs_width, FreeMono_Bold10_ascii_33fs_height, FreeMono_Bold10_ascii_33fs_bits},
   {FreeMono_Bold10_ascii_34fs_width, FreeMono_Bold10_ascii_34fs_height, FreeMono_Bold10_ascii_34fs_bits},
   {FreeMono_Bold10_ascii_35fs_width, FreeMono_Bold10_ascii_35fs_height, FreeMono_Bold10_ascii_35fs_bits},
   {FreeMono_Bold10_ascii_36fs_width, FreeMono_Bold10_ascii_36fs_height, FreeMono_Bold10_ascii_36fs_bits},
   {FreeMono_Bold10_ascii_37fs_width, FreeMono_Bold10_ascii_37fs_height, FreeMono_Bold10_ascii_37fs_bits},
   {FreeMono_Bold10_ascii_38fs_width, FreeMono_Bold10_ascii_38fs_height, FreeMono_Bold10_ascii_38fs_bits},
   {FreeMono_Bold10_ascii_39fs_width, FreeMono_Bold10_ascii_39fs_height, FreeMono_Bold10_ascii_39fs_bits},
   {FreeMono_Bold10_ascii_40fs_width, FreeMono_Bold10_ascii_40fs_height, FreeMono_Bold10_ascii_40fs_bits},
   {FreeMono_Bold10_ascii_41fs_width, FreeMono_Bold10_ascii_41fs_height, FreeMono_Bold10_ascii_41fs_bits},
   {FreeMono_Bold10_ascii_42fs_width, FreeMono_Bold10_ascii_42fs_height, FreeMono_Bold10_ascii_42fs_bits},
   {FreeMono_Bold10_ascii_43fs_width, FreeMono_Bold10_ascii_43fs_height, FreeMono_Bold10_ascii_43fs_bits},
   {FreeMono_Bold10_ascii_44fs_width, FreeMono_Bold10_ascii_44fs_height, FreeMono_Bold10_ascii_44fs_bits},
   {FreeMono_Bold10_ascii_45fs_width, FreeMono_Bold10_ascii_45fs_height, FreeMono_Bold10_ascii_45fs_bits},
   {FreeMono_Bold10_ascii_46fs_width, FreeMono_Bold10_ascii_46fs_height, FreeMono_Bold10_ascii_46fs_bits},
   {FreeMono_Bold10_ascii_47fs_width, FreeMono_Bold10_ascii_47fs_height, FreeMono_Bold10_ascii_47fs_bits},
   {FreeMono_Bold10_0fs_width, FreeMono_Bold10_0fs_height, FreeMono_Bold10_0fs_bits},
   {FreeMono_Bold10_1fs_width, FreeMono_Bold10_1fs_height, FreeMono_Bold10_1fs_bits},
   {FreeMono_Bold10_2fs_width, FreeMono_Bold10_2fs_height, FreeMono_Bold10_2fs_bits},
   {FreeMono_Bold10_3fs_width, FreeMono_Bold10_3fs_height, FreeMono_Bold10_3fs_bits},
   {FreeMono_Bold10_4fs_width, FreeMono_Bold10_4fs_height, FreeMono_Bold10_4fs_bits},
   {FreeMono_Bold10_5fs_width, FreeMono_Bold10_5fs_height, FreeMono_Bold10_5fs_bits},
   {FreeMono_Bold10_6fs_width, FreeMono_Bold10_6fs_height, FreeMono_Bold10_6fs_bits},
   {FreeMono_Bold10_7fs_width, FreeMono_Bold10_7fs_height, FreeMono_Bold10_7fs_bits},
   {FreeMono_Bold10_8fs_width, FreeMono_Bold10_8fs_height, FreeMono_Bold10_8fs_bits},
   {FreeMono_Bold10_9fs_width, FreeMono_Bold10_9fs_height, FreeMono_Bold10_9fs_bits},
   {FreeMono_Bold10_ascii_58fs_width, FreeMono_Bold10_ascii_58fs_height, FreeMono_Bold10_ascii_58fs_bits},
   {FreeMono_Bold10_ascii_59fs_width, FreeMono_Bold10_ascii_59fs_height, FreeMono_Bold10_ascii_59fs_bits},
   {FreeMono_Bold10_ascii_60fs_width, FreeMono_Bold10_ascii_60fs_height, FreeMono_Bold10_ascii_60fs_bits},
   {FreeMono_Bold10_ascii_61fs_width, FreeMono_Bold10_ascii_61fs_height, FreeMono_Bold10_ascii_61fs_bits},
   {FreeMono_Bold10_ascii_62fs_width, FreeMono_Bold10_ascii_62fs_height, FreeMono_Bold10_ascii_62fs_bits},
   {FreeMono_Bold10_ascii_63fs_width, FreeMono_Bold10_ascii_63fs_height, FreeMono_Bold10_ascii_63fs_bits},
   {FreeMono_Bold10_ascii_64fs_width, FreeMono_Bold10_ascii_64fs_height, FreeMono_Bold10_ascii_64fs_bits},
   {FreeMono_Bold10_Afs_width, FreeMono_Bold10_Afs_height, FreeMono_Bold10_Afs_bits},
   {FreeMono_Bold10_Bfs_width, FreeMono_Bold10_Bfs_height, FreeMono_Bold10_Bfs_bits},
   {FreeMono_Bold10_Cfs_width, FreeMono_Bold10_Cfs_height, FreeMono_Bold10_Cfs_bits},
   {FreeMono_Bold10_Dfs_width, FreeMono_Bold10_Dfs_height, FreeMono_Bold10_Dfs_bits},
   {FreeMono_Bold10_Efs_width, FreeMono_Bold10_Efs_height, FreeMono_Bold10_Efs_bits},
   {FreeMono_Bold10_Ffs_width, FreeMono_Bold10_Ffs_height, FreeMono_Bold10_Ffs_bits},
   {FreeMono_Bold10_Gfs_width, FreeMono_Bold10_Gfs_height, FreeMono_Bold10_Gfs_bits},
   {FreeMono_Bold10_Hfs_width, FreeMono_Bold10_Hfs_height, FreeMono_Bold10_Hfs_bits},
   {FreeMono_Bold10_Ifs_width, FreeMono_Bold10_Ifs_height, FreeMono_Bold10_Ifs_bits},
   {FreeMono_Bold10_Jfs_width, FreeMono_Bold10_Jfs_height, FreeMono_Bold10_Jfs_bits},
   {FreeMono_Bold10_Kfs_width, FreeMono_Bold10_Kfs_height, FreeMono_Bold10_Kfs_bits},
   {FreeMono_Bold10_Lfs_width, FreeMono_Bold10_Lfs_height, FreeMono_Bold10_Lfs_bits},
   {FreeMono_Bold10_Mfs_width, FreeMono_Bold10_Mfs_height, FreeMono_Bold10_Mfs_bits},
   {FreeMono_Bold10_Nfs_width, FreeMono_Bold10_Nfs_height, FreeMono_Bold10_Nfs_bits},
   {FreeMono_Bold10_Ofs_width, FreeMono_Bold10_Ofs_height, FreeMono_Bold10_Ofs_bits},
   {FreeMono_Bold10_Pfs_width, FreeMono_Bold10_Pfs_height, FreeMono_Bold10_Pfs_bits},
   {FreeMono_Bold10_Qfs_width, FreeMono_Bold10_Qfs_height, FreeMono_Bold10_Qfs_bits},
   {FreeMono_Bold10_Rfs_width, FreeMono_Bold10_Rfs_height, FreeMono_Bold10_Rfs_bits},
   {FreeMono_Bold10_Sfs_width, FreeMono_Bold10_Sfs_height, FreeMono_Bold10_Sfs_bits},
   {FreeMono_Bold10_Tfs_width, FreeMono_Bold10_Tfs_height, FreeMono_Bold10_Tfs_bits},
   {FreeMono_Bold10_Ufs_width, FreeMono_Bold10_Ufs_height, FreeMono_Bold10_Ufs_bits},
   {FreeMono_Bold10_Vfs_width, FreeMono_Bold10_Vfs_height, FreeMono_Bold10_Vfs_bits},
   {FreeMono_Bold10_Wfs_width, FreeMono_Bold10_Wfs_height, FreeMono_Bold10_Wfs_bits},
   {FreeMono_Bold10_Xfs_width, FreeMono_Bold10_Xfs_height, FreeMono_Bold10_Xfs_bits},
   {FreeMono_Bold10_Yfs_width, FreeMono_Bold10_Yfs_height, FreeMono_Bold10_Yfs_bits},
   {FreeMono_Bold10_Zfs_width, FreeMono_Bold10_Zfs_height, FreeMono_Bold10_Zfs_bits},
   {FreeMono_Bold10_ascii_91fs_width, FreeMono_Bold10_ascii_91fs_height, FreeMono_Bold10_ascii_91fs_bits},
   {FreeMono_Bold10_ascii_92fs_width, FreeMono_Bold10_ascii_92fs_height, FreeMono_Bold10_ascii_92fs_bits},
   {FreeMono_Bold10_ascii_93fs_width, FreeMono_Bold10_ascii_93fs_height, FreeMono_Bold10_ascii_93fs_bits},
   {FreeMono_Bold10_ascii_94fs_width, FreeMono_Bold10_ascii_94fs_height, FreeMono_Bold10_ascii_94fs_bits},
   {FreeMono_Bold10_ascii_95fs_width, FreeMono_Bold10_ascii_95fs_height, FreeMono_Bold10_ascii_95fs_bits},
   {FreeMono_Bold10_ascii_96fs_width, FreeMono_Bold10_ascii_96fs_height, FreeMono_Bold10_ascii_96fs_bits},
   {FreeMono_Bold10_afs_width, FreeMono_Bold10_afs_height, FreeMono_Bold10_afs_bits},
   {FreeMono_Bold10_bfs_width, FreeMono_Bold10_bfs_height, FreeMono_Bold10_bfs_bits},
   {FreeMono_Bold10_cfs_width, FreeMono_Bold10_cfs_height, FreeMono_Bold10_cfs_bits},
   {FreeMono_Bold10_dfs_width, FreeMono_Bold10_dfs_height, FreeMono_Bold10_dfs_bits},
   {FreeMono_Bold10_efs_width, FreeMono_Bold10_efs_height, FreeMono_Bold10_efs_bits},
   {FreeMono_Bold10_ffs_width, FreeMono_Bold10_ffs_height, FreeMono_Bold10_ffs_bits},
   {FreeMono_Bold10_gfs_width, FreeMono_Bold10_gfs_height, FreeMono_Bold10_gfs_bits},
   {FreeMono_Bold10_hfs_width, FreeMono_Bold10_hfs_height, FreeMono_Bold10_hfs_bits},
   {FreeMono_Bold10_ifs_width, FreeMono_Bold10_ifs_height, FreeMono_Bold10_ifs_bits},
   {FreeMono_Bold10_jfs_width, FreeMono_Bold10_jfs_height, FreeMono_Bold10_jfs_bits},
   {FreeMono_Bold10_kfs_width, FreeMono_Bold10_kfs_height, FreeMono_Bold10_kfs_bits},
   {FreeMono_Bold10_lfs_width, FreeMono_Bold10_lfs_height, FreeMono_Bold10_lfs_bits},
   {FreeMono_Bold10_mfs_width, FreeMono_Bold10_mfs_height, FreeMono_Bold10_mfs_bits},
   {FreeMono_Bold10_nfs_width, FreeMono_Bold10_nfs_height, FreeMono_Bold10_nfs_bits},
   {FreeMono_Bold10_ofs_width, FreeMono_Bold10_ofs_height, FreeMono_Bold10_ofs_bits},
   {FreeMono_Bold10_pfs_width, FreeMono_Bold10_pfs_height, FreeMono_Bold10_pfs_bits},
   {FreeMono_Bold10_qfs_width, FreeMono_Bold10_qfs_height, FreeMono_Bold10_qfs_bits},
   {FreeMono_Bold10_rfs_width, FreeMono_Bold10_rfs_height, FreeMono_Bold10_rfs_bits},
   {FreeMono_Bold10_sfs_width, FreeMono_Bold10_sfs_height, FreeMono_Bold10_sfs_bits},
   {FreeMono_Bold10_tfs_width, FreeMono_Bold10_tfs_height, FreeMono_Bold10_tfs_bits},
   {FreeMono_Bold10_ufs_width, FreeMono_Bold10_ufs_height, FreeMono_Bold10_ufs_bits},
   {FreeMono_Bold10_vfs_width, FreeMono_Bold10_vfs_height, FreeMono_Bold10_vfs_bits},
   {FreeMono_Bold10_wfs_width, FreeMono_Bold10_wfs_height, FreeMono_Bold10_wfs_bits},
   {FreeMono_Bold10_xfs_width, FreeMono_Bold10_xfs_height, FreeMono_Bold10_xfs_bits},
   {FreeMono_Bold10_yfs_width, FreeMono_Bold10_yfs_height, FreeMono_Bold10_yfs_bits},
   {FreeMono_Bold10_zfs_width, FreeMono_Bold10_zfs_height, FreeMono_Bold10_zfs_bits},
   {FreeMono_Bold10_ascii_123fs_width, FreeMono_Bold10_ascii_123fs_height, FreeMono_Bold10_ascii_123fs_bits},
   {FreeMono_Bold10_ascii_124fs_width, FreeMono_Bold10_ascii_124fs_height, FreeMono_Bold10_ascii_124fs_bits},
   {FreeMono_Bold10_ascii_125fs_width, FreeMono_Bold10_ascii_125fs_height, FreeMono_Bold10_ascii_125fs_bits},
   {FreeMono_Bold10_ascii_126fs_width, FreeMono_Bold10_ascii_126fs_height, FreeMono_Bold10_ascii_126fs_bits} 
}; 

/*
 * XBM font data
 */
#include "FreeMono_Bold30_font.xbm"
/* array of glyphs */
static glyph FreeMono_Bold30_glyphsArray[] = {
   {FreeMono_Bold30_ascii_33fs_width, FreeMono_Bold30_ascii_33fs_height, FreeMono_Bold30_ascii_33fs_bits},
   {FreeMono_Bold30_ascii_34fs_width, FreeMono_Bold30_ascii_34fs_height, FreeMono_Bold30_ascii_34fs_bits},
   {FreeMono_Bold30_ascii_35fs_width, FreeMono_Bold30_ascii_35fs_height, FreeMono_Bold30_ascii_35fs_bits},
   {FreeMono_Bold30_ascii_36fs_width, FreeMono_Bold30_ascii_36fs_height, FreeMono_Bold30_ascii_36fs_bits},
   {FreeMono_Bold30_ascii_37fs_width, FreeMono_Bold30_ascii_37fs_height, FreeMono_Bold30_ascii_37fs_bits},
   {FreeMono_Bold30_ascii_38fs_width, FreeMono_Bold30_ascii_38fs_height, FreeMono_Bold30_ascii_38fs_bits},
   {FreeMono_Bold30_ascii_39fs_width, FreeMono_Bold30_ascii_39fs_height, FreeMono_Bold30_ascii_39fs_bits},
   {FreeMono_Bold30_ascii_40fs_width, FreeMono_Bold30_ascii_40fs_height, FreeMono_Bold30_ascii_40fs_bits},
   {FreeMono_Bold30_ascii_41fs_width, FreeMono_Bold30_ascii_41fs_height, FreeMono_Bold30_ascii_41fs_bits},
   {FreeMono_Bold30_ascii_42fs_width, FreeMono_Bold30_ascii_42fs_height, FreeMono_Bold30_ascii_42fs_bits},
   {FreeMono_Bold30_ascii_43fs_width, FreeMono_Bold30_ascii_43fs_height, FreeMono_Bold30_ascii_43fs_bits},
   {FreeMono_Bold30_ascii_44fs_width, FreeMono_Bold30_ascii_44fs_height, FreeMono_Bold30_ascii_44fs_bits},
   {FreeMono_Bold30_ascii_45fs_width, FreeMono_Bold30_ascii_45fs_height, FreeMono_Bold30_ascii_45fs_bits},
   {FreeMono_Bold30_ascii_46fs_width, FreeMono_Bold30_ascii_46fs_height, FreeMono_Bold30_ascii_46fs_bits},
   {FreeMono_Bold30_ascii_47fs_width, FreeMono_Bold30_ascii_47fs_height, FreeMono_Bold30_ascii_47fs_bits},
   {FreeMono_Bold30_0fs_width, FreeMono_Bold30_0fs_height, FreeMono_Bold30_0fs_bits},
   {FreeMono_Bold30_1fs_width, FreeMono_Bold30_1fs_height, FreeMono_Bold30_1fs_bits},
   {FreeMono_Bold30_2fs_width, FreeMono_Bold30_2fs_height, FreeMono_Bold30_2fs_bits},
   {FreeMono_Bold30_3fs_width, FreeMono_Bold30_3fs_height, FreeMono_Bold30_3fs_bits},
   {FreeMono_Bold30_4fs_width, FreeMono_Bold30_4fs_height, FreeMono_Bold30_4fs_bits},
   {FreeMono_Bold30_5fs_width, FreeMono_Bold30_5fs_height, FreeMono_Bold30_5fs_bits},
   {FreeMono_Bold30_6fs_width, FreeMono_Bold30_6fs_height, FreeMono_Bold30_6fs_bits},
   {FreeMono_Bold30_7fs_width, FreeMono_Bold30_7fs_height, FreeMono_Bold30_7fs_bits},
   {FreeMono_Bold30_8fs_width, FreeMono_Bold30_8fs_height, FreeMono_Bold30_8fs_bits},
   {FreeMono_Bold30_9fs_width, FreeMono_Bold30_9fs_height, FreeMono_Bold30_9fs_bits},
   {FreeMono_Bold30_ascii_58fs_width, FreeMono_Bold30_ascii_58fs_height, FreeMono_Bold30_ascii_58fs_bits},
   {FreeMono_Bold30_ascii_59fs_width, FreeMono_Bold30_ascii_59fs_height, FreeMono_Bold30_ascii_59fs_bits},
   {FreeMono_Bold30_ascii_60fs_width, FreeMono_Bold30_ascii_60fs_height, FreeMono_Bold30_ascii_60fs_bits},
   {FreeMono_Bold30_ascii_61fs_width, FreeMono_Bold30_ascii_61fs_height, FreeMono_Bold30_ascii_61fs_bits},
   {FreeMono_Bold30_ascii_62fs_width, FreeMono_Bold30_ascii_62fs_height, FreeMono_Bold30_ascii_62fs_bits},
   {FreeMono_Bold30_ascii_63fs_width, FreeMono_Bold30_ascii_63fs_height, FreeMono_Bold30_ascii_63fs_bits},
   {FreeMono_Bold30_ascii_64fs_width, FreeMono_Bold30_ascii_64fs_height, FreeMono_Bold30_ascii_64fs_bits},
   {FreeMono_Bold30_Afs_width, FreeMono_Bold30_Afs_height, FreeMono_Bold30_Afs_bits},
   {FreeMono_Bold30_Bfs_width, FreeMono_Bold30_Bfs_height, FreeMono_Bold30_Bfs_bits},
   {FreeMono_Bold30_Cfs_width, FreeMono_Bold30_Cfs_height, FreeMono_Bold30_Cfs_bits},
   {FreeMono_Bold30_Dfs_width, FreeMono_Bold30_Dfs_height, FreeMono_Bold30_Dfs_bits},
   {FreeMono_Bold30_Efs_width, FreeMono_Bold30_Efs_height, FreeMono_Bold30_Efs_bits},
   {FreeMono_Bold30_Ffs_width, FreeMono_Bold30_Ffs_height, FreeMono_Bold30_Ffs_bits},
   {FreeMono_Bold30_Gfs_width, FreeMono_Bold30_Gfs_height, FreeMono_Bold30_Gfs_bits},
   {FreeMono_Bold30_Hfs_width, FreeMono_Bold30_Hfs_height, FreeMono_Bold30_Hfs_bits},
   {FreeMono_Bold30_Ifs_width, FreeMono_Bold30_Ifs_height, FreeMono_Bold30_Ifs_bits},
   {FreeMono_Bold30_Jfs_width, FreeMono_Bold30_Jfs_height, FreeMono_Bold30_Jfs_bits},
   {FreeMono_Bold30_Kfs_width, FreeMono_Bold30_Kfs_height, FreeMono_Bold30_Kfs_bits},
   {FreeMono_Bold30_Lfs_width, FreeMono_Bold30_Lfs_height, FreeMono_Bold30_Lfs_bits},
   {FreeMono_Bold30_Mfs_width, FreeMono_Bold30_Mfs_height, FreeMono_Bold30_Mfs_bits},
   {FreeMono_Bold30_Nfs_width, FreeMono_Bold30_Nfs_height, FreeMono_Bold30_Nfs_bits},
   {FreeMono_Bold30_Ofs_width, FreeMono_Bold30_Ofs_height, FreeMono_Bold30_Ofs_bits},
   {FreeMono_Bold30_Pfs_width, FreeMono_Bold30_Pfs_height, FreeMono_Bold30_Pfs_bits},
   {FreeMono_Bold30_Qfs_width, FreeMono_Bold30_Qfs_height, FreeMono_Bold30_Qfs_bits},
   {FreeMono_Bold30_Rfs_width, FreeMono_Bold30_Rfs_height, FreeMono_Bold30_Rfs_bits},
   {FreeMono_Bold30_Sfs_width, FreeMono_Bold30_Sfs_height, FreeMono_Bold30_Sfs_bits},
   {FreeMono_Bold30_Tfs_width, FreeMono_Bold30_Tfs_height, FreeMono_Bold30_Tfs_bits},
   {FreeMono_Bold30_Ufs_width, FreeMono_Bold30_Ufs_height, FreeMono_Bold30_Ufs_bits},
   {FreeMono_Bold30_Vfs_width, FreeMono_Bold30_Vfs_height, FreeMono_Bold30_Vfs_bits},
   {FreeMono_Bold30_Wfs_width, FreeMono_Bold30_Wfs_height, FreeMono_Bold30_Wfs_bits},
   {FreeMono_Bold30_Xfs_width, FreeMono_Bold30_Xfs_height, FreeMono_Bold30_Xfs_bits},
   {FreeMono_Bold30_Yfs_width, FreeMono_Bold30_Yfs_height, FreeMono_Bold30_Yfs_bits},
   {FreeMono_Bold30_Zfs_width, FreeMono_Bold30_Zfs_height, FreeMono_Bold30_Zfs_bits},
   {FreeMono_Bold30_ascii_91fs_width, FreeMono_Bold30_ascii_91fs_height, FreeMono_Bold30_ascii_91fs_bits},
   {FreeMono_Bold30_ascii_92fs_width, FreeMono_Bold30_ascii_92fs_height, FreeMono_Bold30_ascii_92fs_bits},
   {FreeMono_Bold30_ascii_93fs_width, FreeMono_Bold30_ascii_93fs_height, FreeMono_Bold30_ascii_93fs_bits},
   {FreeMono_Bold30_ascii_94fs_width, FreeMono_Bold30_ascii_94fs_height, FreeMono_Bold30_ascii_94fs_bits},
   {FreeMono_Bold30_ascii_95fs_width, FreeMono_Bold30_ascii_95fs_height, FreeMono_Bold30_ascii_95fs_bits},
   {FreeMono_Bold30_ascii_96fs_width, FreeMono_Bold30_ascii_96fs_height, FreeMono_Bold30_ascii_96fs_bits},
   {FreeMono_Bold30_afs_width, FreeMono_Bold30_afs_height, FreeMono_Bold30_afs_bits},
   {FreeMono_Bold30_bfs_width, FreeMono_Bold30_bfs_height, FreeMono_Bold30_bfs_bits},
   {FreeMono_Bold30_cfs_width, FreeMono_Bold30_cfs_height, FreeMono_Bold30_cfs_bits},
   {FreeMono_Bold30_dfs_width, FreeMono_Bold30_dfs_height, FreeMono_Bold30_dfs_bits},
   {FreeMono_Bold30_efs_width, FreeMono_Bold30_efs_height, FreeMono_Bold30_efs_bits},
   {FreeMono_Bold30_ffs_width, FreeMono_Bold30_ffs_height, FreeMono_Bold30_ffs_bits},
   {FreeMono_Bold30_gfs_width, FreeMono_Bold30_gfs_height, FreeMono_Bold30_gfs_bits},
   {FreeMono_Bold30_hfs_width, FreeMono_Bold30_hfs_height, FreeMono_Bold30_hfs_bits},
   {FreeMono_Bold30_ifs_width, FreeMono_Bold30_ifs_height, FreeMono_Bold30_ifs_bits},
   {FreeMono_Bold30_jfs_width, FreeMono_Bold30_jfs_height, FreeMono_Bold30_jfs_bits},
   {FreeMono_Bold30_kfs_width, FreeMono_Bold30_kfs_height, FreeMono_Bold30_kfs_bits},
   {FreeMono_Bold30_lfs_width, FreeMono_Bold30_lfs_height, FreeMono_Bold30_lfs_bits},
   {FreeMono_Bold30_mfs_width, FreeMono_Bold30_mfs_height, FreeMono_Bold30_mfs_bits},
   {FreeMono_Bold30_nfs_width, FreeMono_Bold30_nfs_height, FreeMono_Bold30_nfs_bits},
   {FreeMono_Bold30_ofs_width, FreeMono_Bold30_ofs_height, FreeMono_Bold30_ofs_bits},
   {FreeMono_Bold30_pfs_width, FreeMono_Bold30_pfs_height, FreeMono_Bold30_pfs_bits},
   {FreeMono_Bold30_qfs_width, FreeMono_Bold30_qfs_height, FreeMono_Bold30_qfs_bits},
   {FreeMono_Bold30_rfs_width, FreeMono_Bold30_rfs_height, FreeMono_Bold30_rfs_bits},
   {FreeMono_Bold30_sfs_width, FreeMono_Bold30_sfs_height, FreeMono_Bold30_sfs_bits},
   {FreeMono_Bold30_tfs_width, FreeMono_Bold30_tfs_height, FreeMono_Bold30_tfs_bits},
   {FreeMono_Bold30_ufs_width, FreeMono_Bold30_ufs_height, FreeMono_Bold30_ufs_bits},
   {FreeMono_Bold30_vfs_width, FreeMono_Bold30_vfs_height, FreeMono_Bold30_vfs_bits},
   {FreeMono_Bold30_wfs_width, FreeMono_Bold30_wfs_height, FreeMono_Bold30_wfs_bits},
   {FreeMono_Bold30_xfs_width, FreeMono_Bold30_xfs_height, FreeMono_Bold30_xfs_bits},
   {FreeMono_Bold30_yfs_width, FreeMono_Bold30_yfs_height, FreeMono_Bold30_yfs_bits},
   {FreeMono_Bold30_zfs_width, FreeMono_Bold30_zfs_height, FreeMono_Bold30_zfs_bits},
   {FreeMono_Bold30_ascii_123fs_width, FreeMono_Bold30_ascii_123fs_height, FreeMono_Bold30_ascii_123fs_bits},
   {FreeMono_Bold30_ascii_124fs_width, FreeMono_Bold30_ascii_124fs_height, FreeMono_Bold30_ascii_124fs_bits},
   {FreeMono_Bold30_ascii_125fs_width, FreeMono_Bold30_ascii_125fs_height, FreeMono_Bold30_ascii_125fs_bits},
   {FreeMono_Bold30_ascii_126fs_width, FreeMono_Bold30_ascii_126fs_height, FreeMono_Bold30_ascii_126fs_bits} 
}; 

/*
 * XBM font data
 */
#include "FreeMono_Bold50_font.xbm"
/* array of glyphs  */
static glyph FreeMono_Bold50_glyphsArray[] = {
   {font_ascii_33fs_width, font_ascii_33fs_height, font_ascii_33fs_bits},
   {font_ascii_34fs_width, font_ascii_34fs_height, font_ascii_34fs_bits},
   {font_ascii_35fs_width, font_ascii_35fs_height, font_ascii_35fs_bits},
   {font_ascii_36fs_width, font_ascii_36fs_height, font_ascii_36fs_bits},
   {font_ascii_37fs_width, font_ascii_37fs_height, font_ascii_37fs_bits},
   {font_ascii_38fs_width, font_ascii_38fs_height, font_ascii_38fs_bits},
   {font_ascii_39fs_width, font_ascii_39fs_height, font_ascii_39fs_bits},
   {font_ascii_40fs_width, font_ascii_40fs_height, font_ascii_40fs_bits},
   {font_ascii_41fs_width, font_ascii_41fs_height, font_ascii_41fs_bits},
   {font_ascii_42fs_width, font_ascii_42fs_height, font_ascii_42fs_bits},
   {font_ascii_43fs_width, font_ascii_43fs_height, font_ascii_43fs_bits},
   {font_ascii_44fs_width, font_ascii_44fs_height, font_ascii_44fs_bits},
   {font_ascii_45fs_width, font_ascii_45fs_height, font_ascii_45fs_bits},
   {font_ascii_46fs_width, font_ascii_46fs_height, font_ascii_46fs_bits},
   {font_ascii_47fs_width, font_ascii_47fs_height, font_ascii_47fs_bits},
   {_0fs_width, _0fs_height, _0fs_bits},
   {_1fs_width, _1fs_height, _1fs_bits},
   {_2fs_width, _2fs_height, _2fs_bits},
   {_3fs_width, _3fs_height, _3fs_bits},
   {_4fs_width, _4fs_height, _4fs_bits},
   {_5fs_width, _5fs_height, _5fs_bits},
   {_6fs_width, _6fs_height, _6fs_bits},
   {_7fs_width, _7fs_height, _7fs_bits},
   {_8fs_width, _8fs_height, _8fs_bits},
   {_9fs_width, _9fs_height, _9fs_bits},
   {font_ascii_58fs_width, font_ascii_58fs_height, font_ascii_58fs_bits},
   {font_ascii_59fs_width, font_ascii_59fs_height, font_ascii_59fs_bits},
   {font_ascii_60fs_width, font_ascii_60fs_height, font_ascii_60fs_bits},
   {font_ascii_61fs_width, font_ascii_61fs_height, font_ascii_61fs_bits},
   {font_ascii_62fs_width, font_ascii_62fs_height, font_ascii_62fs_bits},
   {font_ascii_63fs_width, font_ascii_63fs_height, font_ascii_63fs_bits},
   {font_ascii_64fs_width, font_ascii_64fs_height, font_ascii_64fs_bits},
   {Afs_width, Afs_height, Afs_bits},
   {Bfs_width, Bfs_height, Bfs_bits},
   {Cfs_width, Cfs_height, Cfs_bits},
   {Dfs_width, Dfs_height, Dfs_bits},
   {Efs_width, Efs_height, Efs_bits},
   {Ffs_width, Ffs_height, Ffs_bits},
   {Gfs_width, Gfs_height, Gfs_bits},
   {Hfs_width, Hfs_height, Hfs_bits},
   {Ifs_width, Ifs_height, Ifs_bits},
   {Jfs_width, Jfs_height, Jfs_bits},
   {Kfs_width, Kfs_height, Kfs_bits},
   {Lfs_width, Lfs_height, Lfs_bits},
   {Mfs_width, Mfs_height, Mfs_bits},
   {Nfs_width, Nfs_height, Nfs_bits},
   {Ofs_width, Ofs_height, Ofs_bits},
   {Pfs_width, Pfs_height, Pfs_bits},
   {Qfs_width, Qfs_height, Qfs_bits},
   {Rfs_width, Rfs_height, Rfs_bits},
   {Sfs_width, Sfs_height, Sfs_bits},
   {Tfs_width, Tfs_height, Tfs_bits},
   {Ufs_width, Ufs_height, Ufs_bits},
   {Vfs_width, Vfs_height, Vfs_bits},
   {Wfs_width, Wfs_height, Wfs_bits},
   {Xfs_width, Xfs_height, Xfs_bits},
   {Yfs_width, Yfs_height, Yfs_bits},
   {Zfs_width, Zfs_height, Zfs_bits},
   {font_ascii_91fs_width, font_ascii_91fs_height, font_ascii_91fs_bits},
   {font_ascii_92fs_width, font_ascii_92fs_height, font_ascii_92fs_bits},
   {font_ascii_93fs_width, font_ascii_93fs_height, font_ascii_93fs_bits},
   {font_ascii_94fs_width, font_ascii_94fs_height, font_ascii_94fs_bits},
   {font_ascii_95fs_width, font_ascii_95fs_height, font_ascii_95fs_bits},
   {font_ascii_96fs_width, font_ascii_96fs_height, font_ascii_96fs_bits},
   {afs_width, afs_height, afs_bits},
   {bfs_width, bfs_height, bfs_bits},
   {cfs_width, cfs_height, cfs_bits},
   {dfs_width, dfs_height, dfs_bits},
   {efs_width, efs_height, efs_bits},
   {ffs_width, ffs_height, ffs_bits},
   {gfs_width, gfs_height, gfs_bits},
   {hfs_width, hfs_height, hfs_bits},
   {ifs_width, ifs_height, ifs_bits},
   {jfs_width, jfs_height, jfs_bits},
   {kfs_width, kfs_height, kfs_bits},
   {lfs_width, lfs_height, lfs_bits},
   {mfs_width, mfs_height, mfs_bits},
   {nfs_width, nfs_height, nfs_bits},
   {ofs_width, ofs_height, ofs_bits},
   {pfs_width, pfs_height, pfs_bits},
   {qfs_width, qfs_height, qfs_bits},
   {rfs_width, rfs_height, rfs_bits},
   {sfs_width, sfs_height, sfs_bits},
   {tfs_width, tfs_height, tfs_bits},
   {ufs_width, ufs_height, ufs_bits},
   {vfs_width, vfs_height, vfs_bits},
   {wfs_width, wfs_height, wfs_bits},
   {xfs_width, xfs_height, xfs_bits},
   {yfs_width, yfs_height, yfs_bits},
   {zfs_width, zfs_height, zfs_bits},
   {font_ascii_123fs_width, font_ascii_123fs_height, font_ascii_123fs_bits},
   {font_ascii_124fs_width, font_ascii_124fs_height, font_ascii_124fs_bits},
   {font_ascii_125fs_width, font_ascii_125fs_height, font_ascii_125fs_bits},
   {font_ascii_126fs_width, font_ascii_126fs_height, font_ascii_126fs_bits} 
};

/*
 * font type global variable
 */
static glyph* glyphsArray = FreeMono_Bold10_glyphsArray;

/* 
 * choose a type of font
 */
int dib_ChooseFont(int option)
{
   if (option == 0)
   {
      glyphsArray = FreeMono_Bold10_glyphsArray;
   } 
   else if (option == 1)
   {
      glyphsArray = FreeMono_Bold30_glyphsArray;
   }
   else if (option == 2)
   {
      glyphsArray = FreeMono_Bold50_glyphsArray;
   }
   else
   {
      return -1;
   }

   return 1;
}

/*! \brief  Draws a single XBM character or glyph.
 *
 * @param image  [in] a pointer to a device independent bitmap (pDIB)
 * @param font   [in] name of the XBM image glyph we want to draw
 * @param px,py  [in] bottom left corner to copy the XBM glyph in 'image'
 * @param fw,fh  [in] with and height of the XBM 'font' we want to draw
 * @param colour [in] array of unsigned bytes storing the colour of the glyph
 *
 * @return Given a pointer to a device independent bitmap (pDIB) and a string 
 *         on exit the glyph stored in 'font' XBM  is drawn across the bitmap 
 *         starting at (px py). It returns 0 if there are no errors, -1 otherwise.
 *
 * \warning Note The the 'image' DIB pointer has to be allocated using a did 
 *          library function, such as `dib_create`. 
 *
 * \note XBM bitmaps are stored in three files: 
 *          FreeMono_Bold10_font.xbm, FreeMono_Bold30_font.xbm, and
 *          FreeMono_Bold50_font.xbm. Each XBM image with, heigh and name is
 *          contained in these files; XBM names as well as their corresponding 
 *          width and heights are accessed by three global arrays of `glyph` 
 *          data records and these are: 
 *          FreeMono_Bold10_glyphsArray
 *          FreeMono_Bold30_glyphsArray
 *          FreeMono_Bold50_glyphsArray
 * 
 */
static int dib_DrawCharacter(pdib image, 
                  unsigned char* font, 
                  int px, int py, 
                  int fw, int fh, 
                  UBYTE* colour)
{
   int depth = dib_Bitcount(image); /* check image depth */
   if (depth != 32 && depth != 24)
   {
      printf("Error in DrawCharacter:");
      printf("It currently does not support %d-bit bitmaps\n", depth);
      return -1;
   } 
   int byte_count = depth/8;
   int width  = dib_Width(image);
   int height = dib_Height(image);
   int storage_width = dib_StorageWidth(image);
   unsigned char* buffer = dib_Image(image);

   /* 
    * calculate how many bytes make  
    * a scanline of the font's bitmap. 
    */
   int text_scanline = fw / 8;
   if (fw % 8 != 0) text_scanline++;
   /* clip against the left image edge */
   /* 
    * left edge offset used to clip 
    * the 1-bit bitmap font angaint the image 
    */
   int tx_off=0; 
   if (px < 0)
   {
      if (px + fw < 0)
      {
         return 0;
      }
      tx_off = -px;
      fw = px + fw;
      px = 0;
   }
   /* clip against the right image edge */
   if (px + fw >= width)
   {
      if (px >= width)
      {
         return 0;
      }
      fw = width - px;
   }
   /* clip against the top top image edge */
   if (py + fh >= height)
   {
      if (py >= height)
      {
         return 0;
      }
      fh = height - py;
   }
   /* clip against the bottom image edge */
   /* 
    * left edge offset used to clip 
    * the 1-bit bitmap font angaint the image 
    */
   int ty_off=fh-1; 
   if (py < 0) 
   {
      if (py + fh < 0)
      {
         return 0;
      }
      ty_off = (py + fh) - 1;
      fh = py + fh;
      py = 0;
   }
  
   /* draw the font across the image */
   int y, ty; /* counters */
   for (y=py, ty=ty_off; y < py+fh; y++, ty--)
   {
      int scanline = y * storage_width; /* y offset */
      int x, tx; /* counters */
      for (x=px, tx=tx_off; x < px+fw; x++, tx++)
      {
         int offset = x*byte_count;  /* x offset */
         int xv = font[ty * text_scanline + tx / 8];
         if (xv & (1 << (tx % 8)))
         {
            buffer[ scanline + offset+0]  = colour[0];
            buffer[ scanline + offset+1]  = colour[1];
            buffer[ scanline + offset+2]  = colour[2];
         }
         
      }
   }

   /* Ok! */
   return 0;
}

/*
 * draws a string
 */
int dib_DrawString(pdib image, 
               const char* str, 
               int px, int py, 
               UBYTE* colour)
{
   int posx = px; 
   int posy = py;
   int i; /* counter */
   for (i=0; str[i] != '\0'; i++) 
   {
      if (str[i] == ' ')
      {
         posx += glyphsArray[0].width;
      } 
      else if (str[i] == '\n' || str[i] == '\r')
      {
         posx = px;
         posy -= glyphsArray[0].height;
      } 
      else if (str[i] == '\t')
      {
         posx += 3*glyphsArray[0].width;
      } 
      else if (str[i] == '\v')
      {
         posy -= 3*glyphsArray[0].height;
      } 
      else if (str[i] >= 33 && str[i] <= 126) 
      {
         int g = str[i] - 33; /* calculate glyph index */
         int error = dib_DrawCharacter(image, 
                     glyphsArray[g].name, 
                     posx, posy, 
                     glyphsArray[g].width, glyphsArray[g].height, 
                     colour);
         if (error != 0) /* we have an error */
         {
            /* return error; // abort */
         }
         posx += glyphsArray[g].width;
      }
   }
   /* Ok! */
   return 0;
}

/*
 * get string drawing dimentions
 */
int dib_DrawStringSize(const char* str, int* left, int* up)
{ 
   int   px = 0, py = 0, max_x=0;
   int posx = px, posy = py;
   int i; /* counter */
   for (i=0; str[i] != '\0'; i++) 
   {
      if (str[i] == ' ')
      {
         posx += glyphsArray[0].width;
      } 
      else if (str[i] == '\n' || str[i] == '\r')
      {
         if (max_x < posx) { max_x = posx; }
         posx = px;
         posy -= glyphsArray[0].height;
      } 
      else if (str[i] == '\t')
      {
         posx += 3*glyphsArray[0].width;
      } 
      else if (str[i] == '\v')
      {
         posy -= 3*glyphsArray[0].height;
      } 
      else if (str[i] >= 33 && str[i] <= 126) 
      {
         int g = str[i] - 33; /* calculate glyph index */
         posx += glyphsArray[g].width;
      }
   }
   if (max_x < posx) { max_x = posx; }
   *left= max_x;
   *up= glyphsArray[0].height;
   /* Ok! */
   /* return how many pixels one move down from the starting point */
   return -1*posy; 
}

