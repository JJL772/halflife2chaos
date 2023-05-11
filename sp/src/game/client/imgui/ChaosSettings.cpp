
#include "imgui/imgui.h"
#include "imgui/imgui_window.h"

#include "tier1/convar.h"
#include "tier1/utlvector.h"
#include "vgui/ISurface.h"
#include "vgui_controls/Controls.h"
#include "Color.h"

#include <tier0/memdbgon.h>

class CChaosDurationPage;
class CChaosProbabilityPage;
class CChaosSettingsPage;

struct ChaosCvar_t
{
	const char* label;
	const char* cvar;
};

constexpr ChaosCvar_t CHAOS_TIME_CVARS[] =
{
	{ "Blackhole Duration", "chaos_time_pull_to_player" },
	{ "No Brake Duration", "chaos_time_no_brake" },
	{ "Bumpty Duration", "chaos_time_bumpy" },
	{ "Death Water Duration", "chaos_time_death_water" },
	{ "Quick Clip Disable Duration", "chaos_time_quickclip_off" },
	{ "Double Barrel Shotgun Duration", "chaos_time_barrel_shotgun" },
	{ "Enable Quickclip Duration", "chaos_time_quickclip_on" },
	{ "Fast Physics Duration", "chaos_time_phys_fast" },
	{ "Floor Is Lava Duration", "chaos_time_floor_is_lava" },
	{ "Use Spam Duration", "chaos_time_use_spam" },
	{ "Grenade Guns Duration", "chaos_time_nade_guns" },
	{ "Invert Gravity Duration", "chaos_time_invertg" },
	{ "Laggy NPCs Duration", "chaos_time_interp_npcs" },
	{ "Lock Vehicle Duration", "chaos_time_lock_vehicle" },
	{ "Low Gravity Duration", "chaos_time_lowg" },
	{ "No Climbing Duration", "chaos_time_incline" },
	{ "No Horizontal Mouse Duration", "chaos_time_no_mouse_horizontal" },
	{ "No Vertical Mouse Duration", "chaos_time_no_mouse_vertical" },
	{ "No Reload Duration", "chaos_time_no_reload" },
	{ "No Save Duration", "chaos_time_disable_save" },
	{ "Explode on Death Duration", "chaos_time_explode_on_death" },
	{ "Ortho Cam Duration", "chaos_time_ortho_cam" },
	{ "Pause Physics Duration", "chaos_time_phys_pause" },
	{ "Big Player Duration", "chaos_time_player_big" },
	{ "Small Player Duration", "chaos_time_player_small" },
	{ "Random Colors Duration", "chaos_time_random_colors" },
	{ "Repulsive Duration", "chaos_time_push_from_player" },
	{ "Slow Physics Duration", "chaos_time_phys_slow" },
	{ "Solid Triggers Duration", "chaos_time_solid_triggers" },
	{ "No Movement Duration", "chaos_time_no_movement" },
	{ "Super Gravity Duration", "chaos_time_superg" },
	{ "Supercold Duration", "chaos_time_supercold" },
	{ "Superhot Duration", "chaos_time_superhot" },
	{ "Super Grab Duration", "chaos_time_super_grab" },
	{ "Super Speed Duration", "chaos_time_super_movement" },
	{ "Surprise Reforestation Duration", "chaos_time_forest" },
	{ "Teleporter Bullets Duration", "chaos_time_bullet_teleport" },
	{ "Low Detail Duration", "chaos_time_low_detail" },
	{ "Water World Duration", "chaos_time_swim_in_air" },
	{ "Where Are The Objects? Duration", "chaos_time_only_draw_world" },
	{ "Why so Rushed? Duration", "chaos_time_cant_leave_map" },
	{ "Wobbly Duration", "chaos_time_earthquake" },
	{ "World of Apathy Duration", "chaos_time_npc_neutral" },
	{ "World of Fear Duration", "chaos_time_npc_fear" },
	{ "World of Hate Duration", "chaos_time_npc_hate" },
	{ "World of Love Duration", "chaos_time_npc_like" },
	{ "Vision Machine Broke Duration", "chaos_time_lock_pvs" },
	{ "You Teleport? Duration", "chaos_time_npc_teleport" },
	{ "Zero Gravity Duration", "chaos_time_zerog" },
};

constexpr ChaosCvar_t CHAOS_PROB_CVARS[] =
{
	
};

class CChaosSettingsWindow : public IImguiWindow
{
	struct Item_t
	{
		const char* label;
		ConVarRef ref;
	};
	
	CUtlVector<Item_t> m_DurationItems;
	CUtlVector<Item_t> m_ProbItems;
	
	ImVec2 m_NewPos;
	ImVec2 m_NewSize;
public:
	CChaosSettingsWindow() :
		IImguiWindow( "chaos_settings", "Chaos Settings" )
	{
	}
	
	//---------------------------------------------------------------------------------------//
	// Draw tab bar for the menu
	//---------------------------------------------------------------------------------------//
	bool Draw() override
	{
		// Set initial pos/size
		if ( m_NewSize.x != -1 )
		{
			ImGui::SetWindowPos( m_NewPos );
			ImGui::SetWindowSize( m_NewSize );
			m_NewSize = m_NewPos = { -1, -1 };
		}
		
		if ( ImGui::BeginTabBar( "SettingsTab" ) )
		{
			if ( ImGui::BeginTabItem( "General" ) )
			{
				DrawGeneralPage();
				ImGui::EndTabItem();
			}
			if ( ImGui::BeginTabItem( "Effect Durations" ) )
			{
				DrawDurationsPage();
				ImGui::EndTabItem();
			}
			if ( ImGui::BeginTabItem( "Effect Probabilities" ) )
			{
				DrawProbPage();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		return true;
	}
	
	//---------------------------------------------------------------------------------------//
	// Purpose: Set the default window location and size
	//---------------------------------------------------------------------------------------//
	void OnChangeVisibility() override
	{
		int w, h;
		vgui::surface()->GetScreenSize( w, h );
		const ImVec2 vecWindowSize( 350, 550 );
		m_NewPos = ImVec2( ( w / 2 ) - ( vecWindowSize.x / 2 ), ( h / 2 )- ( vecWindowSize.y / 2 ) );
		m_NewSize = vecWindowSize;
	}
	
	//---------------------------------------------------------------------------------------//
	// Draw general settings page
	//---------------------------------------------------------------------------------------//
	void DrawGeneralPage()
	{
		ImGui::Dummy( ImVec2( 0, 10 ) );
		static ConVarRef chaos( "chaos" );
		bool bEnabled = chaos.GetBool();
		if ( ImGui::Checkbox( "Enable Chaos", &bEnabled ) )
			chaos.SetValue( bEnabled );
		
		static ConVarRef chaos_effect_interval( "chaos_effect_interval" );
		float flInterval = chaos_effect_interval.GetFloat();
		if ( ImGui::SliderFloat( "Effect Interval ", &flInterval, 0, 120 ) )
			chaos_effect_interval.SetValue( flInterval );
		if ( ImGui::IsItemHovered() )
			ImGui::SetTooltip( "The time (in seconds) between effects" );
		
		static ConVarRef chaos_effect_time( "chaos_effect_time" );
		float flTime = chaos_effect_time.GetFloat();
		if ( ImGui::SliderFloat( "Effect Duration", &flTime, 0, 240 ) )
			chaos_effect_time.SetValue( flTime );
			
		static ConVarRef chaos_strike_max( "chaos_strike_max" );
		int nStrikes = chaos_strike_max.GetInt();
		if ( ImGui::InputScalar( "Effect Strikes", ImGuiDataType_S32, &nStrikes ) )
			chaos_strike_max.SetValue( nStrikes );
		if ( ImGui::IsItemHovered() )
			ImGui::SetTooltip( "If an effect causes the player to die this many times, it will be disabled" );
		
		auto colorVec = []( const ConVarRef& r, const ConVarRef& g, const ConVarRef& b, const ConVarRef& a ) {
			return Vector4D(
				r.GetFloat() / 255.f, g.GetFloat() / 255.f, b.GetFloat() / 255.f, a.GetFloat() / 255.f
			);
		};
		
		auto vecColor = []( const Vector4D &vec, ConVarRef& r, ConVarRef& g, ConVarRef& b, ConVarRef& a ) {
			r.SetValue( vec.x * 255.f );
			g.SetValue( vec.y * 255.f );
			b.SetValue( vec.z * 255.f );
			a.SetValue( vec.w * 255.f );
		};
		

		// These should probably just use built-in cvar color support
		static ConVarRef chaos_text_r( "chaos_text_r" );
		static ConVarRef chaos_text_g( "chaos_text_g" );
		static ConVarRef chaos_text_b( "chaos_text_b" );
		static ConVarRef chaos_text_a( "chaos_text_a" );
		
		auto vecTextColor = colorVec( chaos_text_r, chaos_text_g, chaos_text_b, chaos_text_a );
		if ( ImGui::ColorEdit4( "Text Color", vecTextColor.Base(), ImGuiColorEditFlags_Uint8 ) )
			vecColor( vecTextColor, chaos_text_r, chaos_text_g, chaos_text_b, chaos_text_a );
			
		static ConVarRef chaos_textfade_r( "chaos_textfade_r" );
		static ConVarRef chaos_textfade_g( "chaos_textfade_g" );
		static ConVarRef chaos_textfade_b( "chaos_textfade_b" );
		static ConVarRef chaos_textfade_a( "chaos_textfade_a" );

		auto vecFadeColor = colorVec( chaos_textfade_r, chaos_textfade_g, chaos_textfade_b, chaos_textfade_a );
		if ( ImGui::ColorEdit4( "Faded Text Color", vecFadeColor.Base(), ImGuiColorEditFlags_Uint8 ) )
			vecColor( vecFadeColor, chaos_textfade_r, chaos_textfade_g, chaos_textfade_b, chaos_textfade_a );

		static ConVarRef chaos_bar_r( "chaos_bar_r" );
		static ConVarRef chaos_bar_g( "chaos_bar_g" );
		static ConVarRef chaos_bar_b( "chaos_bar_b" );
		static ConVarRef chaos_bar_a( "chaos_bar_a" );
		
		auto vecBarColor = colorVec( chaos_bar_r, chaos_bar_g, chaos_bar_b, chaos_bar_a );
		if ( ImGui::ColorEdit4( "Bar Color", vecBarColor.Base(), ImGuiColorEditFlags_Uint8 ) )
			vecColor( vecBarColor, chaos_bar_r, chaos_bar_g, chaos_bar_b, chaos_bar_a );
	}
	
	
	//---------------------------------------------------------------------------------------//
	// Draws the page with all of our durations
	//---------------------------------------------------------------------------------------//
	void DrawDurationsPage()
	{
		// Init the list if it hasnt been already
		if ( m_DurationItems.IsEmpty() )
		{
			for ( auto dur : CHAOS_TIME_CVARS )
				m_DurationItems.AddToTail( { dur.label, ConVarRef( dur.cvar ) } );
		}
		
		static char szFilter[256];
		ImGui::PushID( "Filter" );
		ImGui::InputTextWithHint( "", "Filter", szFilter, sizeof( szFilter ) );
		ImGui::PopID();
		
		ImGui::Dummy( ImVec2( 0, 10 ) );
		
		static ConVarRef chaos_effect_time( "chaos_effect_time" );
		const float flDuration = chaos_effect_time.GetFloat();

		for ( auto& item : m_DurationItems )
		{
			if ( szFilter[0] && !V_stristr( item.label, szFilter ) )
				continue;

			float flVal = item.ref.GetFloat() * flDuration;
			if ( ImGui::SliderFloat( item.label, &flVal, 0, flDuration ) )
				item.ref.SetValue( flVal / flDuration );
		}
	}
	
	//---------------------------------------------------------------------------------------//
	// Draws the effect propabilities page
	//---------------------------------------------------------------------------------------//
	void DrawProbPage()
	{
		if ( m_ProbItems.IsEmpty() )
		{
			for ( auto p : CHAOS_PROB_CVARS )
				m_ProbItems.AddToTail( { p.label, ConVarRef( p.cvar ) } );
		}
		
		for ( auto& item : m_ProbItems )
		{
			int nVal = item.ref.GetInt();
			if ( ImGui::SliderInt( item.label, &nVal, 0, 100 ) )
				item.ref.SetValue( nVal );
		}
	}
};

static CChaosSettingsWindow s_ChaosSettings;


CON_COMMAND_F( cl_open_chaos_settings, "Show chaos settings panel", FCVAR_CLIENTCMD_CAN_EXECUTE | FCVAR_CLIENTDLL )
{
	g_pImguiSystem->SetWindowVisible( &s_ChaosSettings, true, true );
}

